#include "pch.h"
#include "SpawnerKeeper.h"

#include "Collider.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "Entity.h"
#include "GreenMonster.h"
#include "LevelScene.h"
#include "Transform.h"
#include "utils.h"

SpawnerKeeper::SpawnerKeeper(LevelScene* pScene)
	: m_pScene{ pScene }
	, m_pPlayer{ pScene->GetPlayer()->GetComponent<Transform>() }
{
	const int poolAmount{ 10 };

	m_pZombiePool = new EnemyPool<Zombie>();
	for (int i{ 0 }; i < poolAmount; ++i)
	{
		m_pZombiePool->ReturnObject(Zombie::Create(pScene, m_pZombiePool));
	}

	m_pGreenMonsterPool = new EnemyPool<GreenMonster>();
	for (int i{ 0 }; i < poolAmount; ++i)
	{
		m_pGreenMonsterPool->ReturnObject(GreenMonster::Create(pScene, m_pGreenMonsterPool));
	}
}

SpawnerKeeper::~SpawnerKeeper()
{
	delete m_pZombiePool;
	delete m_pGreenMonsterPool;
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

	utils::SetColor(Color4f(1, 0, 0, 1));
	utils::DrawEllipse(0, 0, m_SetSpawnerRange, m_SetSpawnerRange, 2);

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

	Enemy* pEnemy{ GetPooledEnemy(spawner->GetEnemyType()) };
	if (pEnemy == nullptr)
	{
		std::cout << "Enemy pool ran out" << std::endl;
		return;
	}

	// Set position outside of ground
	const Collider* pEnemyColl{ pEnemy->GetParent()->GetComponent<Collider>() };
	const float halfHeight{ pEnemyColl->GetHeight() / 2.f };
	const Vector2f pos{ Vector2f(spawner->GetPosition().x, spawner->GetPosition().y + halfHeight) };
	pEnemy->GetParent()->GetComponent<Transform>()->SetPosition(pos);

	pEnemy->ResetEnemy();
}

void SpawnerKeeper::UpdateSetSpawners() const
{
	for (EnemySpawner* spawner : m_SetEnemySpawners)
	{
		if (spawner->HasSpawned()) continue;

		const float playerDist{ spawner->GetPosition().GetDistance(m_pPlayer->GetPosition()) };
		if (playerDist > m_SetSpawnerRange) continue;

		Enemy* pEnemy{ GetPooledEnemy(spawner->GetEnemyType()) };
		if (pEnemy == nullptr)
		{
			std::cout << "Enemy pool ran out" << std::endl;
			return;
		}

		// Set position outside of ground
		const Collider* pEnemyColl{ pEnemy->GetParent()->GetComponent<Collider>() };
		const float halfHeight{ pEnemyColl->GetHeight() / 2.f };
		const Vector2f pos{ Vector2f(spawner->GetPosition().x, spawner->GetPosition().y + halfHeight) };
		pEnemy->GetParent()->GetComponent<Transform>()->SetPosition(pos);

		pEnemy->ResetEnemy();

		spawner->SetHasSpawned(true);
	}
}

Enemy* SpawnerKeeper::GetPooledEnemy(EnemyType type) const
{
	Enemy* pEnemy;
	switch (type)
	{
	case EnemyType::zombie:
		pEnemy = m_pZombiePool->GetPooledObject();
		break;
	case EnemyType::greenMonster:
		pEnemy = m_pGreenMonsterPool->GetPooledObject();
		break;
	default:
		pEnemy = nullptr;
	}

	if (pEnemy == nullptr) return nullptr;

	pEnemy->GetParent()->SetActive(true);

	return pEnemy;
}
