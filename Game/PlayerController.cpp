#include "pch.h"
#include "PlayerController.h"

#include <cassert>

#include "AnimatorRenderer.h"
#include "Entity.h"
#include "InputHandler.h"
#include "LadderCollider.h"
#include "LevelScene.h"
#include "PhysicsBody.h"
#include "PhysicsHandler.h"
#include "PlayerCollider.h"
#include "ProjectilePool.h"
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

	if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE] && m_HurtTimer <= 0.f)
	{
		Damage(m_pTransform->GetPosition() + Vector2f(1, 0));
	}

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

	m_pAnimator->SetParameter("isHurt", m_HurtTimer > 0);
	if (m_HurtTimer > 0.f)
	{
		m_HurtTimer -= deltaTime;
		UpdateHurt();
		return;
	}


	UpdateGroundMovement();

	UpdateLadderMovement();

	UpdateJumping();
	UpdateShooting(deltaTime);
}

void PlayerController::Draw() const
{
	// Draw grounded check
	const Vector2f bottomLeft{ m_pTransform->GetPosition() + Vector2f(-m_ColliderWidth / 2 + 1, -m_ColliderHeight / 2 - 0.5f) };
	const Vector2f bottomRight{ bottomLeft + Vector2f(m_ColliderWidth - 2, 0) };

	utils::SetColor(Color4f(0, 1, 0, 1));
	utils::DrawLine(bottomLeft.ToPoint2f(), bottomRight.ToPoint2f());
}

void PlayerController::Damage(Vector2f from)
{
	if (m_HasArmor)
	{
		m_HasArmor = false;
		m_HurtTimer = m_DamagedInactiveTime;
	}
	else
	{
		// Dead
		m_HurtTimer = 100.f;
	}

	const Vector2f hitDirection{ from - m_pTransform->GetPosition() };
	const float directionMultiplier{ hitDirection.x > 0 ? -1.f : 1.f };

	m_IsGrounded = false;
	m_pTransform->MovePosition(Vector2f(0, 1));
	m_pPhysicsBody->SetVelocity(Vector2f(m_DamagedHorizontalVelocity * directionMultiplier, m_DamagedVerticalVelocity));
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
	m_IsCrouched = GetInputHandler()->GetKeyPressed("down") && m_IsGrounded;
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
	if(m_IsGrounded)
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
	const Vector2f bottomLeft{ m_pTransform->GetPosition() + Vector2f(-m_ColliderWidth / 2 + 1, -m_ColliderHeight / 2 - 0.5f) };
	const Vector2f bottomRight{ bottomLeft + Vector2f(m_ColliderWidth - 2, 0) };

	const std::pair<bool, Collider*> result{ GetPhysicsHandler()->Linecast(bottomLeft, bottomRight) };

	m_IsGrounded = result.first;
	m_pAnimator->SetParameter("isGrounded", m_IsGrounded);
}

void PlayerController::UpdateJumping() const
{
	if (m_IsGrounded && GetInputHandler()->GetKeyDown("jump"))
	{
		m_pPhysicsBody->SetYVelocity(m_JumpForce);
		// Move out of ground so next frame isn't considered grounded instantly
		m_pTransform->MovePosition(Vector2f(0, 1));
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

		pool->FireProjectile(Projectile::FireData{
			Projectile::Type::player,
			m_pTransform->GetPosition(),
			Vector2f(m_ShootingSpeed * static_cast<float>(m_LookDir), 0),
			0,
			20.f,
			5.f
			});
	}

	m_pAnimator->SetParameter("isShooting", m_IsShooting);
}

void PlayerController::UpdateHurt()
{
	if (m_IsGrounded && m_HurtTimer > 0.f)
	{
		m_HurtTimer = 0.f;
	}
}
