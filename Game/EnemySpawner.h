#pragma once
#include <Component.h>

#include "Vector2f.h"

enum class EnemyType;
class LevelScene;

enum class SpawnerType
{
	random, set
};

class EnemySpawner final : public Component
{
public:
	EnemySpawner(Entity* pParent, const LevelScene* pScene, SpawnerType spawnerType, EnemyType enemyType);

	const Vector2f& GetPosition() const;
	EnemyType GetEnemyType() const;

	bool HasSpawned() const { return m_HasSpawned; }
	void SetHasSpawned(bool newVal) { m_HasSpawned = newVal; }

private:
	SpawnerType m_SpawnerType;
	EnemyType m_EnemyType;

	bool m_HasSpawned{ false };
};

