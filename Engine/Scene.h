#pragma once

class PhysicsHandler;
class TextureCache;
class EntityKeeper;
class Camera;
class Game;

class Scene
{
public:
	explicit Scene();
	explicit Scene(Camera* pCamera);
	Scene& operator=(const Scene& rhs) = delete;
	Scene& operator=(Scene&& rhs) = delete;
	Scene(const Scene& rhs) = delete;
	Scene(Scene&& rhs) = delete;
	virtual ~Scene();

	void Initialize(Game* pGame);

	void Update(float deltaTime);
	void Draw() const;

	virtual void InitializeScene() = 0;
	virtual void UpdateScene(float deltaTime) = 0;
	virtual void DrawScene() const = 0;

	Game* GetGame() const;

	Camera* GetCamera() const;
	EntityKeeper* GetEntityKeeper() const;
	PhysicsHandler* GetPhysicsHandler() const;
	TextureCache* GetTextureCache() const;

	void MarkForDeletion();
	bool IsMarkedForDeletion() const;

protected:
	Game* m_pGame;

	EntityKeeper* m_pEntityKeeper;
	PhysicsHandler* m_pPhysicsHandler;
	TextureCache* m_pTextureCache;
	Camera* m_pCamera;

	bool m_MarkedForDeletion{ false };
};
