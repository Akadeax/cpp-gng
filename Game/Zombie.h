#pragma once
#include "Enemy.h"
#include "EnemyPool.h"

class TextureCache;
class Collider;
class AnimatorRenderer;
class PhysicsBody;
class Transform;
class LevelScene;
class Texture;

class Zombie final : public Enemy
{
public:
	explicit Zombie(Entity* pParent, LevelScene* pLevelScene, EnemyPool<Zombie>* returnTo);

	void Initialize() override;
	void Update(float deltaTime) override;


	void ResetEnemy() override;

	static Zombie* Create(LevelScene* pScene, EnemyPool<Zombie>* returnTo);
	void Damage() override;

private:

	LevelScene* m_pLevelScene;

	Transform* m_pTransform{ nullptr };
	Collider* m_pCollider{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };
	AnimatorRenderer* m_pAnimator{ nullptr };

	Transform* m_pPlayer{ nullptr };

	EnemyPool<Zombie>* m_ReturnTo;

	int m_WalkingDirMultiplier{ 0 };
	const float m_WalkSpeed{ 50.f };

	bool m_IsDead{ false };
	const float m_DeathTime{ 0.3f };
	float m_CurrentDeathTime{};

	bool m_Spawned{ false };
	const float m_SpawnTime{ 0.5f };
	float m_CurrentSpawnTime{};

	bool m_Despawning{ false };
	const float m_DespawnTime{ 0.5f };
	float m_CurrentDespawnTime{};

	float m_Lifetime{};
	const float m_TimeUntilDespawn{ 7.f };

	const int m_ScoreValue{ 100 };
};
