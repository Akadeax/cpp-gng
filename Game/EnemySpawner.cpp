#include "pch.h"
#include "EnemySpawner.h"

#include "Camera.h"
#include "Entity.h"
#include "LevelScene.h"
#include "SpawnerKeeper.h"
#include "Transform.h"
#include "utils.h"

EnemySpawner::EnemySpawner(Entity* pParent, const LevelScene* pScene, SpawnerType spawnerType, EnemyType enemyType)
	: Component(pParent)
	, m_SpawnerType{ spawnerType }
	, m_EnemyType{ enemyType }
{
	pScene->GetSpawnerKeeper()->AddSpawner(this, m_SpawnerType);
}

const Vector2f& EnemySpawner::GetPosition() const
{
	return GetParent()->GetComponent<Transform>()->GetPosition();
}

EnemyType EnemySpawner::GetEnemyType() const
{
	return m_EnemyType;
}
