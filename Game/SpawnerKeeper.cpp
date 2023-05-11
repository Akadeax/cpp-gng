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
	m_pZombiePool = new EnemyPool<Zombie>();
	for (int i{ 0 }; i < 100; ++i)
	{
		m_pZombiePool->ReturnObject(Zombie::CreateZombie(pScene, m_pZombiePool));
	}
}

SpawnerKeeper::~SpawnerKeeper()
{
	delete m_pZombiePool;
}

void SpawnerKeeper::Update(float deltaTime)
{
	UpdateRandomSpawners(deltaTime);
	UpdateSetSpawners();
}

void SpawnerKeeper::DrawDebugRadius() const
{
	glPushMatrix();

	// Draw random spawn range gizmos
	glTranslatef(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, 0);
	utils::SetColor(Color4f(0, 0, 1, 1));
	utils::DrawEllipse(0, 0, m_RandomMinSpawnRange, m_RandomMinSpawnRange, 2);
	utils::DrawEllipse(0, 0, m_RandomMaxSpawnRange, m_RandomMaxSpawnRange, 2);

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

void SpawnerKeeper::UpdateRandomSpawners(float deltaTime)
{
	if (m_CurrentRandomEnemySpawnTimer > 0.f)
	{
		m_CurrentRandomEnemySpawnTimer -= deltaTime;
		return;
	}

	m_CurrentRandomEnemySpawnTimer = utils::RandomInRange(m_RandomEnemySpawnTimerRange.x, m_RandomEnemySpawnTimerRange.y);

	std::vector<EnemySpawner*> spawnersInRange{};
	for (EnemySpawner* spawner : m_RandomEnemySpawners)
	{
		const Vector2f spawnerPos{ spawner->GetParent()->GetComponent<Transform>()->GetPosition() };
		const float dist{ spawnerPos.GetDistance(m_pPlayer->GetPosition()) };
		if (dist > m_RandomMinSpawnRange && dist < m_RandomMaxSpawnRange)
		{
			spawnersInRange.push_back(spawner);
		}
	}

	if (spawnersInRange.empty()) return;

	const int rndIndex{ utils::RandomInRange(0, static_cast<int>(spawnersInRange.size()) - 1) };
	const EnemySpawner* spawner{ spawnersInRange[rndIndex] };

	const Enemy* pEnemy{ GetPooledEnemy(spawner->GetEnemyType()) };
	if (pEnemy == nullptr)
	{
		std::cout << "Enemy pool ran out" << std::endl;
		return;
	}
	pEnemy->GetParent()->GetComponent<Transform>()->SetPosition(spawner->GetPosition());
}

void SpawnerKeeper::UpdateSetSpawners()
{
	for (EnemySpawner* spawner : m_SetEnemySpawners)
	{
		if (spawner->HasSpawned()) continue;

		const float playerDist{ spawner->GetPosition().GetDistance(m_pPlayer->GetPosition()) };
		if (playerDist > m_SetSpawnerRange) continue;

		const Enemy* pEnemy{ GetPooledEnemy(spawner->GetEnemyType()) };
		if (pEnemy == nullptr)
		{
			std::cout << "Enemy pool ran out" << std::endl;
			return;
		}
		pEnemy->GetParent()->GetComponent<Transform>()->SetPosition(spawner->GetPosition());

		spawner->SetHasSpawned(true);
	}
}

Enemy* SpawnerKeeper::GetPooledEnemy(EnemyType type)
{
	Enemy* pEnemy{};
	switch (type)
	{
	case EnemyType::zombie:
		pEnemy = m_pZombiePool->GetPooledObject();
	}

	if (pEnemy == nullptr) return nullptr;

	pEnemy->GetParent()->SetActive(true);
	pEnemy->ResetEnemy();

	return pEnemy;
}
