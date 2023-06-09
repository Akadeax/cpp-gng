#pragma once

#include "Scene.h"
#include "Vector2f.h"

class HudHandler;
class Collider;
enum class EnemyType;
class SpawnerKeeper;
class ProjectilePool;
class Transform;
class Entity;

class LevelScene final : public Scene
{
public:
	explicit LevelScene() = default;
	LevelScene& operator=(const LevelScene& rhs) = delete;
	LevelScene& operator=(LevelScene&& rhs) = delete;
	LevelScene(const LevelScene& rhs) = delete;
	LevelScene(LevelScene&& rhs) = delete;
	~LevelScene() override;

	void InitializeScene() override;
	void UpdateScene(float deltaTime) override;
	void DrawScene() const override;
	void DrawHud() const override;

	ProjectilePool* GetProjectilePool() const;
	SpawnerKeeper* GetSpawnerKeeper() const;
	HudHandler* GetHudHandler() const;

	Entity* GetPlayer() const;

	void AddForegroundCollider(Vector2f bottomLeft, Vector2f size) const;

private:
	ProjectilePool* m_pProjectilePool{};
	SpawnerKeeper* m_pSpawnerKeeper{};
	HudHandler* m_pHudHandler{};

	Entity* m_pPlayer{};

	Vector2f m_LevelSize{};
	Entity* m_pForeground{};
	Transform* m_pBackgroundTransform{};

	void CreatePlayer();
	void CreateLevel();

	void CreateLadder(float xCoord) const;
	void CreateZombieSpawner(const Vector2f& pos) const;
	void CreateSetSpawner(const Vector2f& pos, EnemyType type) const;

};
