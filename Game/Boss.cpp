#include "pch.h"
#include "Boss.h"

#include <cassert>

#include "Animation.h"
#include "AnimationEndAnimatorTransition.h"
#include "AnimationFrame.h"
#include "AnimatorRenderer.h"
#include "AnimatorState.h"
#include "AnimatorTransition.h"
#include "BossJumpingState.h"
#include "BossShootingState.h"
#include "BossWalkingState.h"
#include "Collider.h"
#include "ConditionalAnimatorTransition.h"
#include "Entity.h"
#include "EntityKeeper.h"
#include "Game.h"
#include "LevelScene.h"
#include "PhysicsBody.h"
#include "SoundHandler.h"
#include "TextureCache.h"
#include "Transform.h"

class Texture;

Boss::Boss(Entity* pParent, EnemyPool<Boss>* returnTo)
	: Enemy(pParent)
	, m_ReturnTo{ returnTo }
{
}

void Boss::ResetEnemy()
{
	m_Health = m_MaxHealth;

	m_IsDead = false;
	m_pAnimator->SetParameter("isDead", false);
	m_CurrentDeathTime = m_DeathTime;
}

void Boss::Damage()
{
	m_Health--;
	if (m_Health <= 0)
	{
		m_IsDead = true;
		m_pAnimator->SetParameter("isDead", true);
		m_pCollider->SetEnabled(false);
		GetSoundHandler()->PlaySoundEffect("bossDeath");
	}
	else
	{
		GetSoundHandler()->PlaySoundEffect("bossHit");
	}
}

void Boss::Initialize()
{
	m_pAnimator = m_pParent->GetComponent<AnimatorRenderer>();
	assert(m_pAnimator != nullptr && "Entity has Boss component but no AnimatorRenderer component");
	m_pCollider = m_pParent->GetComponent<Collider>();
	assert(m_pCollider != nullptr && "Entity has Boss component but no Collider component");
	m_pPhysicsBody = m_pParent->GetComponent<PhysicsBody>();
	assert(m_pPhysicsBody != nullptr && "Entity has Boss component but no PhysicsBody component");
}

void Boss::Update(float deltaTime)
{
	if (m_IsDead)
	{
		if (m_CurrentDeathTime > 0.f)
		{
			m_CurrentDeathTime -= deltaTime;
		}
		else
		{
			m_pParent->SetActive(false);
			m_ReturnTo->ReturnObject(this);

			GetParent()->GetScene()->GetGame()->MarkSceneLoad(new LevelScene());
		}
	}
}

Boss* Boss::Create(LevelScene* pScene, EnemyPool<Boss>* returnTo)
{
	Texture* pTexture{ pScene->GetTextureCache()->LoadTexture("boss", "boss_42x42.png") };
	Entity* pEnemy{ pScene->GetEntityKeeper()->CreateEntity(5, "Enemy") };

	pEnemy->AddComponent(new Transform(pEnemy));

	// RENDERING
	const float spriteWidth{ 42.f };
	const float spriteHeight{ 42.f };

	const std::unordered_map<std::string, AnimatorState*> enemyStates
	{
	{ "jump", new BossJumpingState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 3, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "shoot", new BossShootingState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.3f, Rectf(spriteWidth * 4, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "walk", new BossWalkingState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.2f, Rectf(spriteWidth * 1, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.2f, Rectf(spriteWidth * 2, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "death", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.1f, Rectf(spriteWidth * 5, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 6, spriteHeight * 1, spriteWidth, spriteHeight)),
			new AnimationFrame(0.1f, Rectf(spriteWidth * 7, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	};

	const std::list<AnimatorTransition*> enemyTransitions
	{
		new AnimationEndAnimatorTransition("shoot", "walk"),

		new ConditionalAnimatorTransition("walk", "death", "isDead", true),
		new ConditionalAnimatorTransition("idle", "death", "isDead", true),
		new ConditionalAnimatorTransition("jump", "death", "isDead", true),
	};

	pEnemy->AddComponent(new AnimatorRenderer(
		pEnemy,
		pTexture,
		enemyStates,
		enemyTransitions,
		"walk"
	));

	// LOGIC
	pEnemy->AddComponent(new Collider(pEnemy, std::vector<Vector2f>{
		Vector2f(-spriteWidth / 2, -spriteHeight / 2),
			Vector2f(-spriteWidth / 2, spriteHeight / 2),
			Vector2f(spriteWidth / 2, spriteHeight / 2),
			Vector2f(spriteWidth / 2, -spriteHeight / 2),
	}, true));

	pEnemy->AddComponent(new PhysicsBody(pEnemy));

	pEnemy->AddComponent(new Boss(pEnemy, returnTo));

	pEnemy->Initialize();

	pEnemy->SetActive(false);

	return pEnemy->GetComponent<Boss>();
}
