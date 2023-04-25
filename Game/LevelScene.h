#pragma once

#include "Scene.h"
#include "Vector2f.h"

class SpawnerKeeper;
class Zombie;
class ProjectilePool;
class Transform;
class Entity;

class LevelScene final : public Scene
{
public:
	~LevelScene() override;

	void InitializeScene() override;
	void UpdateScene(float deltaTime) override;
	void DrawScene() const override;

	ProjectilePool* GetProjectilePool() const;
	SpawnerKeeper* GetSpawnerKeeper() const;

	Entity* GetPlayer() const;

private:
	ProjectilePool* m_pProjectilePool{};
	SpawnerKeeper* m_pSpawnerKeeper{};

	Entity* m_pPlayer{};

	Zombie* m_pEnemy{};

	Vector2f m_LevelSize{};
	Transform* m_pBackgroundTransform{};

	void CreatePlayer();
	void CreateEnemy();
	void CreateLevel();

	void CreateLadder(float xCoord) const;
};

