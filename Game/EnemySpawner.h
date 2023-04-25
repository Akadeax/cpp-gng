#pragma once
#include <Component.h>

class LevelScene;

class EnemySpawner final : public Component
{
public:
	EnemySpawner(Entity* pParent, LevelScene* pScene);
	~EnemySpawner() override;

private:
	LevelScene* m_pLevelScene;
};

