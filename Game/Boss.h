#pragma once
#include "Enemy.h"
#include "EnemyPool.h"

class LevelScene;

class Boss final : public Enemy
{
public:
	explicit Boss(Entity* pParent, EnemyPool<Boss>* returnTo);

	void ResetEnemy() override;
	void Damage() override;

	static Boss* Create(LevelScene* pScene, EnemyPool<Boss>* returnTo);

private:
	const int m_MaxHealth{ 7 };
	int m_Health{};

	EnemyPool<Boss>* m_ReturnTo;
};

