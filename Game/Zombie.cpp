#include "pch.h"
#include "Zombie.h"

#include <cassert>

#include "Animation.h"
#include "AnimationFrame.h"
#include "AnimatorAnimationEndTransition.h"
#include "AnimatorRenderer.h"
#include "AnimatorState.h"
#include "Entity.h"
#include "LevelScene.h"
#include "PhysicsBody.h"
#include "Transform.h"
#include "Collider.h"
#include "ConditionalAnimatorTransition.h"
#include "EntityKeeper.h"
#include "TextureCache.h"


Zombie::Zombie(Entity* pParent, LevelScene* pLevelScene, EnemyPool<Zombie>* returnTo)
	: Enemy(pParent)
	, m_pLevelScene{ pLevelScene }
	, m_ReturnTo{ returnTo }
{
}

void Zombie::Initialize()
{
	m_pPlayer = m_pLevelScene->GetPlayer()->GetComponent<Transform>();

	m_pTransform = m_pParent->GetComponent<Transform>();
	assert(m_pTransform != nullptr && "Entity has Zombie component but no Transform component");
	m_pCollider = m_pParent->GetComponent<Collider>();
	assert(m_pCollider != nullptr && "Entity has Zombie component but no Collider component");
	m_pPhysicsBody = m_pParent->GetComponent<PhysicsBody>();
	assert(m_pPhysicsBody != nullptr && "Entity has Zombie component but no PhysicsBody component");
	m_pAnimator = m_pParent->GetComponent<AnimatorRenderer>();
	assert(m_pAnimator != nullptr && "Entity has Zombie component but no AnimatorRenderer component");
}

void Zombie::Update(float deltaTime)
{
	if (m_Spawned && !m_IsDead)
	{
		m_pAnimator->SetFlipX(m_pPhysicsBody->GetVelocity().x < 0);

		const bool isRightOfPlayer{ m_pTransform->GetPosition().x > m_pPlayer->GetPosition().x };
		const float directionTowardsPlayer{ isRightOfPlayer ? -1.f : 1.f };

		m_pPhysicsBody->SetXVelocity(directionTowardsPlayer * m_WalkSpeed);
		return;
	}


	if (!m_Spawned)
	{
		const bool isRightOfPlayer{ m_pTransform->GetPosition().x > m_pPlayer->GetPosition().x };
		m_pAnimator->SetFlipX(isRightOfPlayer);

		if (m_CurrentSpawnTime > 0.f)
		{
			m_CurrentSpawnTime -= deltaTime;
		}
		else
		{
			m_pCollider->SetEnabled(true);
			m_Spawned = true;
		}
	}
	

	if(m_IsDead)
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
}

void Zombie::Damage()
{
	m_pAnimator->SetParameter("isDead", true);

	m_pPhysicsBody->SetXVelocity(0);

	m_pCollider->SetEnabled(false);


	m_CurrentDeathTime = m_DeathTime;
	m_IsDead = true;
}

void Zombie::ResetEnemy()
{
	m_pAnimator->SetState("spawn");

	m_IsDead = false;
	m_pAnimator->SetParameter("isDead", false);

	m_Spawned = false;

	m_pCollider->SetEnabled(false);
	m_CurrentSpawnTime = m_SpawnTime;
}

Zombie* Zombie::CreateZombie(LevelScene* pScene, EnemyPool<Zombie>* returnTo)
{
	Texture* pTexture{ pScene->GetTextureCache()->LoadTexture("zombie", "zombie.png") };
	Entity* pEnemy{ pScene->GetEntityKeeper()->CreateEntity(5, "Enemy") };

	pEnemy->AddComponent(new Transform(pEnemy, Vector2f(150, 55)));

	// RENDERING
	const float spriteWidth{ 22.f };
	const float spriteHeight{ 30.f };

	const std::unordered_map<std::string, AnimatorState*> enemyStates
	{
	{ "spawn", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.1f, Rectf(spriteWidth * 0, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 1, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 2, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 3, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 4, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "walk", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.25f, Rectf(spriteWidth * 5, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.25f, Rectf(spriteWidth * 6, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "despawn", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.1f, Rectf(spriteWidth * 4, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 3, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 2, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 1, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 0, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "death", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.1f, Rectf(spriteWidth * 7, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 8, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 9, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	};

	const std::list<AnimatorTransition*> enemyTransitions
	{
		new AnimatorAnimationEndTransition("spawn", "walk"),
		new ConditionalAnimatorTransition("walk", "death", "isDead", true),
	};

	pEnemy->AddComponent(new AnimatorRenderer(
		pEnemy,
		pTexture,
		enemyStates,
		enemyTransitions,
		"spawn"
	));

	// LOGIC
	pEnemy->AddComponent(new Collider(pEnemy, std::vector<Vector2f>{
		Vector2f(-spriteWidth / 2, -spriteHeight / 2),
			Vector2f(-spriteWidth / 2, spriteHeight / 2),
			Vector2f(spriteWidth / 2, spriteHeight / 2),
			Vector2f(spriteWidth / 2, -spriteHeight / 2),
	}, true));

	pEnemy->AddComponent(new PhysicsBody(pEnemy));

	pEnemy->AddComponent(new Zombie(pEnemy, pScene, returnTo));

	pEnemy->Initialize();

	pEnemy->SetActive(false);

	return pEnemy->GetComponent<Zombie>();
}