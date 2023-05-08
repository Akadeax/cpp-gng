#pragma once
#include <vector>

#include "EnemyPool.h"
#include "Vector2f.h"
#include "Zombie.h"

enum class SpawnerType;
class EnemySpawner;
class Enemy;
class LevelScene;
class Transform;

class SpawnerKeeper
{
public:
	explicit SpawnerKeeper(LevelScene* pScene);

	void Update(float deltaTime);
	void DrawDebugRadius() const;

private:
	LevelScene* m_pScene;
	Transform* m_pPlayer;

	const float m_MinSpawnRange{ 75.f };
	const float m_MaxSpawnRange{ 150.f };

	// Use vector as random range
	const Vector2f m_EnemySpawnTimerRange{ Vector2f(3.f, 7.f) };
	float m_CurrentEnemySpawnTimer{ 0.f };

	std::vector<EnemySpawner*> m_RandomEnemySpawners;
	std::vector<EnemySpawner*> m_SetEnemySpawners;

	EnemyPool<Zombie> m_ZombiePool;

	friend class EnemySpawner;
	void AddSpawner(EnemySpawner* self, SpawnerType type);
	void RemoveSpawner(const EnemySpawner* self, SpawnerType type);

};

