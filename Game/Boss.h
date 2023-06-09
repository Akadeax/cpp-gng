#pragma once
#include "Enemy.h"
#include "EnemyPool.h"

class PhysicsBody;
class Collider;
class AnimatorRenderer;
class LevelScene;

class Boss final : public Enemy
{
public:
	explicit Boss(Entity* pParent, EnemyPool<Boss>* returnTo);

	void ResetEnemy() override;
	void Damage() override;

	void Initialize() override;
	void Update(float deltaTime) override;

	static Boss* Create(LevelScene* pScene, EnemyPool<Boss>* returnTo);

private:
	const int m_MaxHealth{ 7 };
	int m_Health{};

	AnimatorRenderer* m_pAnimator{ nullptr };
	Collider* m_pCollider{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };

	EnemyPool<Boss>* m_ReturnTo;

	bool m_IsDead{ false };
	const float m_DeathTime{ 0.3f };
	float m_CurrentDeathTime{};

};

