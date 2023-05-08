#pragma once
#include "Enemy.h"

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
	explicit Zombie(Entity* pParent, LevelScene* pLevelScene);
	void Initialize() override;
	void Update(float deltaTime) override;


	void ResetEnemy() override;

	static Zombie* CreateZombie(LevelScene* pScene);
	void Damage() override;

private:

	LevelScene* m_pLevelScene;

	Transform* m_pTransform{ nullptr };
	Collider* m_pCollider{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };
	AnimatorRenderer* m_pAnimator{ nullptr };

	Transform* m_pPlayer{ nullptr };

	const float m_WalkSpeed{ 30.f };

	bool m_IsDead{ false };
	const float m_DeathTime{ 0.3f };
	float m_CurrentDeathTime{};

	bool m_Spawned{ false };
	const float m_SpawnTime{ 0.5f };
	float m_CurrentSpawnTime{};
};

