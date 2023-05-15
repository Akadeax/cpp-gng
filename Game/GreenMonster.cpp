#include "pch.h"
#include "GreenMonster.h"

#include <cassert>

#include "Animation.h"
#include "AnimationFrame.h"
#include "AnimationEndAnimatorTransition.h"
#include "AnimatorRenderer.h"
#include "AnimatorState.h"
#include "Collider.h"
#include "ConditionalAnimatorTransition.h"
#include "Entity.h"
#include "EntityKeeper.h"
#include "LevelScene.h"
#include "PhysicsBody.h"
#include "ProjectilePool.h"
#include "Texture.h"
#include "TextureCache.h"
#include "Transform.h"

GreenMonster::GreenMonster(Entity* pParent, LevelScene* pLevelScene, EnemyPool<GreenMonster>* returnTo)
	: Enemy(pParent)
	, m_pLevelScene{ pLevelScene }
	, m_ReturnTo{ returnTo }
{
}

void GreenMonster::Initialize()
{
	m_pPlayer = m_pLevelScene->GetPlayer()->GetComponent<Transform>();

	m_pTransform = m_pParent->GetComponent<Transform>();
	assert(m_pTransform != nullptr && "Entity has GreenMonster component but no Transform component");
	m_pCollider = m_pParent->GetComponent<Collider>();
	assert(m_pCollider != nullptr && "Entity has GreenMonster component but no Collider component");
	m_pAnimator = m_pParent->GetComponent<AnimatorRenderer>();
	assert(m_pAnimator != nullptr && "Entity has GreenMonster component but no AnimatorRenderer component");
}

void GreenMonster::Update(float deltaTime)
{
	if (m_IsDead)
	{
		if (m_CurrentDeathTime > 0.f)
		{
			m_CurrentDeathTime -= deltaTime;
		}
		else
		{
			GetParent()->SetActive(false);
			m_ReturnTo->ReturnObject(this);
		}
	}
	else
	{
		if (m_CurrentShootTime > 0.f)
		{
			m_CurrentShootTime -= deltaTime;
		}
		else
		{
			Shoot();
			m_CurrentShootTime = m_ShootTime;
		}

		const float xDiff{ m_pTransform->GetPosition().x - m_pPlayer->GetPosition().x };
		m_pAnimator->SetFlipX(xDiff < 0);
	}
}

void GreenMonster::ResetEnemy()
{
	m_CurrentShootTime = 0.f;
}

void GreenMonster::Damage()
{
	m_pAnimator->SetParameter("isDead", true);
	m_pCollider->SetEnabled(false);

	m_CurrentDeathTime = m_DeathTime;
	m_IsDead = true;
}

void GreenMonster::Shoot() const
{
	m_pAnimator->SetState("shoot");

	const Vector2f dirTowardsPlayer{ (m_pPlayer->GetPosition() - m_pTransform->GetPosition()).Normalized() };

	const Projectile::FireData data
	{
		Projectile::Type::enemy,
		m_pTransform->GetPosition(),
		dirTowardsPlayer * m_ProjectileVelocity,
		1,
		5, 5
	};

	m_pLevelScene->GetProjectilePool()->FireProjectile(data);
}

GreenMonster* GreenMonster::Create(LevelScene* pScene, EnemyPool<GreenMonster>* returnTo)
{
	Texture* pTexture{ pScene->GetTextureCache()->LoadTexture("greenMonster", "greenMonster_20x28.png") };

	Entity* pEnemy{ pScene->GetEntityKeeper()->CreateEntity(5, "Enemy") };

	pEnemy->AddComponent(new Transform(pEnemy));

	// RENDERING
	const float spriteWidth { 20.f };
	const float spriteHeight{ 28.f };

	const std::unordered_map<std::string, AnimatorState*> enemyStates
	{
	{ "idle", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 0, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "shoot", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.3f, Rectf(spriteWidth * 1, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "death", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.1f, Rectf(spriteWidth * 2, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 3, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 4, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	};

	const std::list<AnimatorTransition*> enemyTransitions
	{
		new AnimationEndAnimatorTransition("shoot", "idle"),
		new ConditionalAnimatorTransition("idle", "death", "isDead", true),
		new ConditionalAnimatorTransition("shoot", "death", "isDead", true),
	};

	pEnemy->AddComponent(new AnimatorRenderer(
		pEnemy,
		pTexture,
		enemyStates,
		enemyTransitions,
		"idle"
	));

	// LOGIC
	pEnemy->AddComponent(new Collider(pEnemy, std::vector<Vector2f>{
		Vector2f(-spriteWidth / 2, -spriteHeight / 2),
			Vector2f(-spriteWidth / 2, spriteHeight / 2),
			Vector2f(spriteWidth / 2, spriteHeight / 2),
			Vector2f(spriteWidth / 2, -spriteHeight / 2),
	}, true));

	pEnemy->AddComponent(new PhysicsBody(pEnemy));

	pEnemy->AddComponent(new GreenMonster(pEnemy, pScene, returnTo));

	pEnemy->Initialize();

	pEnemy->SetActive(false);

	return pEnemy->GetComponent<GreenMonster>();
}
