#include "pch.h"
#include "SpawnerKeeper.h"

#include "EnemySpawner.h"
#include "Entity.h"
#include "LevelScene.h"
#include "Transform.h"

SpawnerKeeper::SpawnerKeeper(LevelScene* pScene)
	: m_pScene{ pScene }
	, m_pPlayer{ pScene->GetPlayer()->GetComponent<Transform>() }
{
}

void SpawnerKeeper::Update(float deltaTime)
{
	if (m_CurrentEnemySpawnTimer > 0.f)
	{
		m_CurrentEnemySpawnTimer -= deltaTime;
		return;
	}

	m_CurrentEnemySpawnTimer = m_EnemySpawnTimer;

	std::vector<EnemySpawner*> spawnersInRange{};
	for (EnemySpawner* spawner : m_EnemySpawners)
	{
		const Vector2f spawnerPos{ spawner->GetParent()->GetComponent<Transform>()->GetPosition() };
		if (spawnerPos.GetDistance(spawnerPos) < m_SpawnRange)
		{
			spawnersInRange.push_back(spawner);
		}
	}

	// TODO: Spawn enemy at random spawner in range
}

void SpawnerKeeper::AddSpawner(EnemySpawner* self)
{
	m_EnemySpawners.push_back(self);
}

void SpawnerKeeper::RemoveSpawner(const EnemySpawner* self)
{
	m_EnemySpawners.erase(std::find(m_EnemySpawners.begin(), m_EnemySpawners.end(), self));
}
