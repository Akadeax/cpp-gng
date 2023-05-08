#pragma once
#include "Component.h"

enum class EnemyType
{
	zombie
};

class Enemy : public Component
{
public:
	explicit Enemy(Entity* pParent);

	virtual void ResetEnemy() = 0;
	virtual void Damage() = 0;
};

