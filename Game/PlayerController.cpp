#include "pch.h"
#include "PlayerController.h"

#include <cassert>

#include "AnimatorRenderer.h"
#include "Entity.h"
#include "Game.h"
#include "InputHandler.h"
#include "LadderCollider.h"
#include "LevelScene.h"
#include "PhysicsBody.h"
#include "PhysicsHandler.h"
#include "PlayerCollider.h"
#include "ProjectilePool.h"
#include "SoundHandler.h"
#include "TextureCache.h"
#include "Transform.h"
#include "utils.h"

class AnimatorRenderer;

PlayerController::PlayerController(Entity* parent)
	: Component(parent)
{
}

void PlayerController::Initialize()
{
	m_pPhysicsBody = m_pParent->GetComponent<PhysicsBody>();
	assert(m_pPhysicsBody != nullptr && "Entity has PlayerController component but not PhysicsBody component");
	m_pTransform = m_pParent->GetComponent<Transform>();
	assert(m_pTransform != nullptr && "Entity has PlayerController component but no Transform component");
	m_pCollider = m_pParent->GetComponent<PlayerCollider>();
	assert(m_pCollider != nullptr && "Entity has PlayerController component but no PlayerCollider component");
	m_pAnimator = m_pParent->GetComponent<AnimatorRenderer>();
	assert(m_pAnimator != nullptr && "Entity has PlayerController component but no AnimatorRenderer component");

	m_pCollider->SetBaseVertices(std::vector<Vector2f>{
		Vector2f(-m_ColliderWidth / 2, -m_ColliderHeight / 2),
		Vector2f(-m_ColliderWidth / 2, m_ColliderHeight / 2),
		Vector2f(m_ColliderWidth / 2, m_ColliderHeight / 2),
		Vector2f(m_ColliderWidth / 2, -m_ColliderHeight / 2)
	});
}

void PlayerController::Update(float deltaTime)
{
	CheckGrounded(deltaTime);

	if (!m_IsGrounded && !m_IsClimbing)
	{
		// Apply gravity if we're not grounded
		m_pPhysicsBody->AddVelocity(Vector2f(0, -m_GravityScale * deltaTime));
	}
	else if (m_IsGrounded && !m_IsClimbing)
	{
		// Reset velocity, otherwise gravity pulls into the ground;
		// if we'd reset to 0 the ground would never register collision
		m_pPhysicsBody->SetYVelocity(-5);
	}
	else if (m_IsClimbing)
	{
		m_pPhysicsBody->SetYVelocity(0);
	}

	if (m_IsDead)
	{
		m_DeathResetTime -= deltaTime;

		if (m_DeathResetTime < 0.f)
		{
			GetParent()->GetScene()->GetGame()->MarkSceneLoad(new LevelScene());
		}

		if (m_IsGrounded)
		{
			m_pAnimator->SetState("deadGround");
			m_pPhysicsBody->SetXVelocity(0);
			m_pCollider->SetCanInteract(false);
		}

		return;
	}

	m_pAnimator->SetParameter("isHurt", m_HurtTimer > 0);
	if (m_HurtTimer > 0.f)
	{
		m_HurtTimer -= deltaTime;
		UpdateHurt(deltaTime);
		return;
	}

	UpdateGroundMovement();

	UpdateLadderMovement();

	UpdateJumping();
	UpdateShooting(deltaTime);

	if (m_pTransform->GetPosition().y < -10.f)
	{
		m_IsDead = true;
	}
}

void PlayerController::Draw() const
{
}

void PlayerController::Damage(const Vector2f& from)
{
	m_pCollider->SetCanInteract(false);

	if (m_HasArmor)
	{
		m_HasArmor = false;
		m_HurtTimer = m_DamagedInactiveTime;
		m_pAnimator->SetTexture(GetTextureCache()->GetTexture("playerNaked"));
		GetSoundHandler()->PlaySoundEffect("arthurHit");
	}
	else
	{
		// Dead
		m_HurtTimer = FLT_MAX;
		m_IsDead = true;
		m_pAnimator->SetState("deadAir");
		GetSoundHandler()->StopAll();
		GetSoundHandler()->PlaySoundEffect("arthurDeath");
	}

	const Vector2f hitDirection{ from - m_pTransform->GetPosition() };
	const float directionMultiplier{ hitDirection.x > 0 ? -1.f : 1.f };

	m_IsGrounded = false;
	const float depenetrationAmount{ 2.f };
	m_pTransform->MovePosition(Vector2f(static_cast<float>(utils::Sign(-hitDirection.x)) * depenetrationAmount, depenetrationAmount));
	m_pPhysicsBody->SetVelocity(Vector2f(m_DamagedHorizontalVelocity * directionMultiplier, m_DamagedVerticalVelocity));

	m_IsClimbing = false;
	m_pCollider->SetTrigger(false);
	m_pAnimator->SetParameter("isClimbing", false);
	m_pAnimator->SetPaused(false);
}

void PlayerController::UpdateGroundMovement()
{
	int moveDir{ GetInputHandler()->GetAxis("left", "right") };

	if (moveDir != 0 && !m_IsClimbing)
	{
		m_LookDir = moveDir;
	}

	m_pAnimator->SetFlipX(m_LookDir != 1);
	m_pAnimator->SetParameter("isWalking", moveDir != 0);

	// Crouching
	m_IsCrouched = GetInputHandler()->GetKeyPressed("down") && m_IsGrounded && !m_IsClimbing;
	m_pAnimator->SetParameter("isCrouched", m_IsCrouched);

	if(m_IsCrouched)
	{
		moveDir = 0;
		m_pCollider->GetBaseVertices()[1].y = -m_ColliderHeight / 2 + m_ColliderHeight * m_CrouchedHitboxHeightMultiplier;
		m_pCollider->GetBaseVertices()[2].y = -m_ColliderHeight / 2 + m_ColliderHeight * m_CrouchedHitboxHeightMultiplier;
	}
	else
	{
		m_pCollider->GetBaseVertices()[1].y = m_ColliderHeight / 2;
		m_pCollider->GetBaseVertices()[2].y = m_ColliderHeight / 2;
	}

	// Disallow changing X velocity while jumping
	if(m_IsGrounded && !m_IsClimbing)
	{
		m_pPhysicsBody->SetXVelocity(m_MovementSpeed * static_cast<float>(moveDir));
	}

	if(m_IsShooting && m_IsGrounded)
	{
		m_pPhysicsBody->SetXVelocity(0);
	}
}

void PlayerController::UpdateLadderMovement()
{
	if ((!m_IsGrounded && !m_IsClimbing) || m_IsShooting) return;

	const int ladderInput{ GetInputHandler()->GetAxis("down", "up") };

	const bool isMovingOnLadder{ m_pCollider->IsTouchingLadder() && ladderInput != 0 };
	if (isMovingOnLadder)
	{
		const bool reachedBottom{ m_pCollider->GetTouchedLadder()->IsPlayerAtBottom() && ladderInput == -1 };
		const bool reachedTop{ m_pCollider->GetTouchedLadder()->IsPlayerAtTop() && ladderInput == 1 };

		if (reachedBottom || reachedTop)
		{
			m_IsClimbing = false;
			m_pCollider->SetTrigger(false);
		}
		else
		{
			m_IsClimbing = true;
			m_pCollider->SetTrigger(true);


			m_pPhysicsBody->SetXVelocity(0);
			m_pTransform->SetXPosition(m_pCollider->GetTouchedLadder()->GetLadderX());

			m_pPhysicsBody->SetYVelocity(m_ClimbSpeed * static_cast<float>(ladderInput));
		}
	}
	else if (!m_pCollider->IsTouchingLadder() && m_IsClimbing)
	{
		m_IsClimbing = false;
		m_pCollider->SetTrigger(false);
	}

	m_pAnimator->SetParameter("isClimbing", m_IsClimbing);
	m_pAnimator->SetParameter("climbDirection", ladderInput);

	if(m_IsClimbing)
	{
		m_pAnimator->SetPaused(ladderInput == 0);
	}
}

void PlayerController::CheckGrounded(float deltaTime)
{
	// Grounded check
	const Vector2f bottomLeft{ m_pTransform->GetPosition() + Vector2f(-m_ColliderWidth / 2 + 1, -m_ColliderHeight / 2 - 1.f) };
	const Vector2f bottomRight{ bottomLeft + Vector2f(m_ColliderWidth - 2, 0) };

	const std::pair<bool, Collider*> result{ GetPhysicsHandler()->Linecast(bottomLeft, bottomRight) };

	m_IsGrounded = result.first;
	m_pAnimator->SetParameter("isGrounded", m_IsGrounded);
}

void PlayerController::UpdateJumping() const
{
	if (!m_IsClimbing && m_IsGrounded && GetInputHandler()->GetKeyDown("jump"))
	{
		m_pPhysicsBody->SetYVelocity(m_JumpForce);
		// Move out of ground so next frame isn't considered grounded instantly
		m_pTransform->MovePosition(Vector2f(0, 1));

		GetSoundHandler()->PlaySoundEffect("arthurJump");
	}
}

void PlayerController::UpdateShooting(float deltaTime)
{
	bool canShoot;

	if (m_CurrentShootCooldown > 0)
	{
		m_CurrentShootCooldown -= deltaTime;
		canShoot = false;
	}
	else
	{
		canShoot = true;
	}


	if (m_CurrentShootTime > 0)
	{
		if (m_CurrentShootTime - deltaTime <= 0)
		{
			m_IsShooting = false;
		}
		m_CurrentShootTime -= deltaTime;
	}
	else if (canShoot && !m_IsClimbing && GetInputHandler()->GetKeyDown("fire"))
	{
		m_CurrentShootTime = m_ShootTime;
		m_CurrentShootCooldown = m_ShootCooldown;
		m_IsShooting = true;

		const ProjectilePool* pool{ dynamic_cast<LevelScene*>(m_pParent->GetScene())->GetProjectilePool() };

		Vector2f projectilePos{ m_pTransform->GetPosition() };
		if (m_IsCrouched)
		{
			projectilePos.y += m_ShootProjectileOffsetCrouched;
		}
		else
		{
			projectilePos.y += m_ShootProjectileOffset;
		}

		pool->FireProjectile(Projectile::FireData{
			Projectile::Type::player,
			projectilePos,
			Vector2f(m_ShootingSpeed * static_cast<float>(m_LookDir), 0),
			0,
			20.f,
			5.f
		});

		GetSoundHandler()->PlaySoundEffect("arthurThrow");
	}

	m_pAnimator->SetParameter("isShooting", m_IsShooting);

}

void PlayerController::UpdateHurt(float deltaTime)
{
	if (m_IsGrounded && m_HurtTimer > 0.f)
	{
		m_HurtTimer = 0.f;
		m_pCollider->SetCanInteract(false);
	}

	if (m_HurtTimer - deltaTime <= 0.f)
	{
		m_pCollider->SetCanInteract(true);
	}
}
