#pragma once
#include "Enemy.h"
#include "EnemyPool.h"

class Collider;
class Transform;
class AnimatorRenderer;
class LevelScene;

class GreenMonster final : public Enemy
{
public:
	explicit GreenMonster(Entity* pParent, LevelScene* pLevelScene, EnemyPool<GreenMonster>* returnTo);

	void Initialize() override;
	void Update(float deltaTime) override;


	void ResetEnemy() override;

	static GreenMonster* Create(LevelScene* pScene, EnemyPool<GreenMonster>* returnTo);
	void Damage() override;

private:
	LevelScene* m_pLevelScene;

	Transform* m_pTransform{ nullptr };
	Collider* m_pCollider{ nullptr };
	AnimatorRenderer* m_pAnimator{ nullptr };

	Transform* m_pPlayer{ nullptr };

	EnemyPool<GreenMonster>* m_ReturnTo;

	const float m_ShootTime{ 3.f };
	float m_CurrentShootTime{};
	void Shoot() const;

	const float m_ProjectileVelocity{ 60.f };

	bool m_IsDead{ false };
	const float m_DeathTime{ 0.3f };
	float m_CurrentDeathTime{};
};

