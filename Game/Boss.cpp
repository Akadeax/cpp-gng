#include "pch.h"
#include "Boss.h"

#include "Animation.h"
#include "AnimationEndAnimatorTransition.h"
#include "AnimationFrame.h"
#include "AnimatorRenderer.h"
#include "AnimatorState.h"
#include "AnimatorTransition.h"
#include "Collider.h"
#include "ConditionalAnimatorTransition.h"
#include "Entity.h"
#include "EntityKeeper.h"
#include "LevelScene.h"
#include "PhysicsBody.h"
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
}

void Boss::Damage()
{
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

	{ "idle", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 0, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "jump", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 3, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "walk", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
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
		new ConditionalAnimatorTransition("idle", "walk", "isWalking", true),
		new ConditionalAnimatorTransition("walk", "idle", "isWalking", false),
		new ConditionalAnimatorTransition("idle", "jump", "isGrounded", false),
		new ConditionalAnimatorTransition("jump", "idle", "isGrounded", true),

		new ConditionalAnimatorTransition("walk", "death", "isDead", true),
		new ConditionalAnimatorTransition("idle", "death", "isDead", true),
		new ConditionalAnimatorTransition("jump", "death", "isDead", true),
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

	pEnemy->AddComponent(new Boss(pEnemy, returnTo));

	pEnemy->Initialize();

	pEnemy->SetActive(false);

	return pEnemy->GetComponent<Boss>();
}
