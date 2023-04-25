#include "pch.h"
#include "EnemySpawner.h"

#include "LevelScene.h"
#include "SpawnerKeeper.h"

EnemySpawner::EnemySpawner(Entity* pParent, LevelScene* pScene)
	: Component(pParent)
	, m_pLevelScene{ pScene }
{
	pScene->GetSpawnerKeeper()->AddSpawner(this);
}

EnemySpawner::~EnemySpawner()
{
	m_pLevelScene->GetSpawnerKeeper()->AddSpawner(this);
}

