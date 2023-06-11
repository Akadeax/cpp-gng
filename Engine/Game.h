#pragma once

#include "structs.h"
#include "SDL.h"

class SoundHandler;
class InputHandler;
class Scene;

class Game
{
public:
	explicit Game(const Window& window, Scene* pStartScene);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	virtual ~Game();

	void InitializeGameEngine();
	void Run();

	virtual void InitializeGame() = 0;
	virtual void UpdateGame(float deltaTime) = 0;
	virtual void DrawGame() const = 0;

	const Rectf& GetViewPort() const
	{
		return m_Viewport;
	}

	InputHandler* GetInputHandler() const;
	SoundHandler* GetSoundHandler() const;

	void MarkSceneLoad(Scene* pScene);


protected:
	// The window properties
	const Window m_Window;
	const Rectf m_Viewport;
	// The window we render to
	SDL_Window* m_pWindow;
	// OpenGL context
	SDL_GLContext m_pContext;
	// Init info
	bool m_Initialized;
	// Prevent timing jumps when debugging
	const float m_MaxElapsedSeconds;

	Scene* m_pCurrentScene;
	InputHandler* m_pInputHandler;
	SoundHandler* m_pSoundHandler;

	// FUNCTIONS
	void CleanupGameEngine();

	void Update(float deltaTime);
	void Draw() const;

	Scene* m_pSceneLoadBuffer{ nullptr };
};
