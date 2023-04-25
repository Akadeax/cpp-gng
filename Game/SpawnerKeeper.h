#pragma once
#include <vector>
class EnemySpawner;
class Enemy;
class LevelScene;
class Transform;

class SpawnerKeeper
{
public:
	explicit SpawnerKeeper(LevelScene* pScene);

	void Update(float deltaTime);

private:
	LevelScene* m_pScene;
	Transform* m_pPlayer;

	const float m_SpawnRange{ 10.f };

	const float m_EnemySpawnTimer{ 5.f };
	float m_CurrentEnemySpawnTimer{ 0.f };

	std::vector<EnemySpawner*> m_EnemySpawners;

	friend class EnemySpawner;
	void AddSpawner(EnemySpawner* self);
	void RemoveSpawner(const EnemySpawner* self);

};

