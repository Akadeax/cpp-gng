#pragma once
#include <vector>

#include "EnemyPool.h"
#include "Vector2f.h"
#include "Zombie.h"

class GreenMonster;
enum class SpawnerType;
class EnemySpawner;
class Enemy;
class LevelScene;
class Transform;

class SpawnerKeeper final
{
public:
	explicit SpawnerKeeper(LevelScene* pScene);
	SpawnerKeeper& operator=(const SpawnerKeeper& rhs) = delete;
	SpawnerKeeper& operator=(SpawnerKeeper&& rhs) = delete;
	SpawnerKeeper(const SpawnerKeeper& rhs) = delete;
	SpawnerKeeper(SpawnerKeeper&& rhs) = delete;
	~SpawnerKeeper();

	void Update(float deltaTime);
	void DrawDebugRadius() const;

private:
	LevelScene* m_pScene;
	Transform* m_pPlayer;

	EnemyPool<Zombie>* m_pZombiePool;
	EnemyPool<GreenMonster>* m_pGreenMonsterPool;

	std::vector<EnemySpawner*> m_RandomEnemySpawners;

	const float m_RandomMinSpawnRange{ 75.f };
	const float m_RandomMaxSpawnRange{ 150.f };

	// Use vector as random range
	const Vector2f m_RandomEnemySpawnTimerRange{ Vector2f(3.f, 7.f) };
	float m_CurrentRandomEnemySpawnTimer{ 0.f };


	std::vector<EnemySpawner*> m_SetEnemySpawners;

	const float m_SetSpawnerRange{ 100.f };


	friend class EnemySpawner;
	void AddSpawner(EnemySpawner* self, SpawnerType type);
	void RemoveSpawner(const EnemySpawner* self, SpawnerType type);

	void UpdateRandomSpawners(float deltaTime);
	void UpdateSetSpawners() const;

	Enemy* GetPooledEnemy(EnemyType type) const;
};
