#include "pch.h"
#include "SpawnerKeeper.h"

#include "Enemy.h"
#include "EnemySpawner.h"
#include "Entity.h"
#include "LevelScene.h"
#include "Transform.h"
#include "utils.h"

SpawnerKeeper::SpawnerKeeper(LevelScene* pScene)
	: m_pScene{ pScene }
	, m_pPlayer{ pScene->GetPlayer()->GetComponent<Transform>() }
{
	m_ZombiePool = EnemyPool<Zombie>();
	for (int i{ 0 }; i < 10; ++i)
	{
		//m_ZombiePool.ReturnObject(Zombie::CreateZombie(pScene));
	}
}

void SpawnerKeeper::Update(float deltaTime)
{
	if (m_CurrentEnemySpawnTimer > 0.f)
	{
		m_CurrentEnemySpawnTimer -= deltaTime;
		return;
	}

	m_CurrentEnemySpawnTimer = utils::RandomInRange(m_EnemySpawnTimerRange.x, m_EnemySpawnTimerRange.y);

	std::vector<EnemySpawner*> spawnersInRange{};
	for (EnemySpawner* spawner : m_RandomEnemySpawners)
	{
		const Vector2f spawnerPos{ spawner->GetParent()->GetComponent<Transform>()->GetPosition() };
		const float dist{ spawnerPos.GetDistance(m_pPlayer->GetPosition()) };
		if (dist > m_MinSpawnRange && dist < m_MaxSpawnRange)
		{
			spawnersInRange.push_back(spawner);
		}
	}

	if (spawnersInRange.empty()) return;

	const int rndIndex{ utils::RandomInRange(0, static_cast<int>(spawnersInRange.size()) - 1) };
	const EnemySpawner* spawner{ spawnersInRange[rndIndex] };
	const Vector2f pos{ spawner->GetPosition() };

	Enemy* enemy{};
	switch (spawner->GetEnemyType())
	{
	case EnemyType::zombie:
		// TODO: Pool
		//enemy = m_ZombiePool.GetPooledObject();
		enemy = Zombie::CreateZombie(m_pScene);
		break;
	}

	enemy->ResetEnemy();
	enemy->GetParent()->GetComponent<Transform>()->SetPosition(pos);
}

void SpawnerKeeper::DrawDebugRadius() const
{
	glPushMatrix();

	// Draw spawn range gizmos
	glTranslatef(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, 0);
	utils::SetColor(Color4f(0, 0, 1, 1));
	utils::DrawEllipse(0, 0, m_MinSpawnRange, m_MinSpawnRange, 2);
	utils::DrawEllipse(0, 0, m_MaxSpawnRange, m_MaxSpawnRange, 2);

	glPopMatrix();

	// Draw spawner gizmos
	for (const EnemySpawner* pSpawner : m_RandomEnemySpawners)
	{
		glPushMatrix();

		const Transform* pSpawnerPos{ pSpawner->GetParent()->GetComponent<Transform>() };
		glTranslatef(pSpawnerPos->GetPosition().x, pSpawnerPos->GetPosition().y, 0);
		utils::SetColor(Color4f(0, 0, 1, 1));
		utils::DrawEllipse(0, 0, 10, 10, 3);

		glPopMatrix();
	}

	for (const EnemySpawner* pSpawner : m_SetEnemySpawners)
	{
		glPushMatrix();

		const Transform* pSpawnerPos{ pSpawner->GetParent()->GetComponent<Transform>() };
		glTranslatef(pSpawnerPos->GetPosition().x, pSpawnerPos->GetPosition().y, 0);
		utils::SetColor(Color4f(1, 0, 0, 1));
		utils::DrawEllipse(0, 0, 10, 10, 3);

		glPopMatrix();
	}
}

void SpawnerKeeper::AddSpawner(EnemySpawner* self, SpawnerType type)
{
	switch (type)
	{
	case SpawnerType::random:
		m_RandomEnemySpawners.push_back(self);
		break;
	case SpawnerType::set:
		m_SetEnemySpawners.push_back(self);
		break;
	}
}

void SpawnerKeeper::RemoveSpawner(const EnemySpawner* self, SpawnerType type)
{
	switch (type)
	{
	case SpawnerType::random:
		m_RandomEnemySpawners.erase(std::find(m_RandomEnemySpawners.begin(), m_RandomEnemySpawners.end(), self));
		break;
	case SpawnerType::set:
		m_SetEnemySpawners.erase(std::find(m_SetEnemySpawners.begin(), m_SetEnemySpawners.end(), self));
		break;
	}
}
