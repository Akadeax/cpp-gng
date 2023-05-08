#include "pch.h"
#include "LevelScene.h"

#include "Animation.h"
#include "AnimationFrame.h"
#include "AnimatorAnimationEndTransition.h"
#include "AnimatorRenderer.h"
#include "AnimatorState.h"
#include "Camera.h"
#include "Collider.h"
#include "ConditionalAnimatorTransition.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "Entity.h"

#include "EntityKeeper.h"
#include "PhysicsBody.h"
#include "PlayerController.h"
#include "Game.h"
#include "LadderCollider.h"
#include "PhysicsHandler.h"
#include "PlayerCamera.h"
#include "PlayerCollider.h"
#include "ProjectilePool.h"
#include "SpawnerKeeper.h"
#include "Texture.h"
#include "TextureCache.h"
#include "Transform.h"

void LevelScene::InitializeScene()
{
	std::cout << "Controls:\nArrows keys: move\nJ: jump\nK: shoot";

	CreatePlayer();

	m_pProjectilePool = new ProjectilePool(this);
	m_pSpawnerKeeper = new SpawnerKeeper(this);

	m_pCamera = new PlayerCamera(m_pPlayer->GetComponent<Transform>());
	// Center camera at start
	m_pCamera->MovePosition(Vector2f(-450, -300));

	CreateLevel();

	const float spawnerDistFromBorder{ 20.f };
	const int spawnerAmount{ 20 };
	const float spawnerIntervalDist{ 60.f };
	const float spawnerHeight{ 55.f };

	for(int i{}; i < spawnerAmount; ++i)
	{
		CreateZombieSpawner(Vector2f(spawnerDistFromBorder + spawnerIntervalDist * static_cast<float>(i), spawnerHeight));
	}
}


LevelScene::~LevelScene()
{
	delete m_pSpawnerKeeper;
	delete m_pProjectilePool;
}

void LevelScene::UpdateScene(float deltaTime)
{
	m_pSpawnerKeeper->Update(deltaTime);

	// Parallax the background
	const float newXPos{ (m_pCamera->GetPosition().x * 0.2f) };
	m_pBackgroundTransform->SetPosition(Vector2f(newXPos, 0));
}

void LevelScene::DrawScene() const
{
	m_pPhysicsHandler->DrawDebugColliders();
	m_pSpawnerKeeper->DrawDebugRadius();
}

ProjectilePool* LevelScene::GetProjectilePool() const
{
	return m_pProjectilePool;
}

SpawnerKeeper* LevelScene::GetSpawnerKeeper() const
{
	return m_pSpawnerKeeper;
}

Entity* LevelScene::GetPlayer() const
{
	return m_pPlayer;
}

void LevelScene::CreatePlayer()
{
	m_pTextureCache->LoadTexture("player", "player.png");

	m_pPlayer = m_pEntityKeeper->CreateEntity(100, "Player");
	m_pPlayer->AddComponent(new Transform(m_pPlayer, Vector2f(10, 40)));

	// RENDERING
	const float spriteWidth{ 22.f };
	const float spriteHeight{ 25.f };

	const std::unordered_map<std::string, AnimatorState*> playerStates
	{
	{ "idle", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 1, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "walk", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.15f, Rectf(spriteWidth * 0, spriteHeight * 2, spriteWidth, spriteHeight)),
			new AnimationFrame(0.15f, Rectf(spriteWidth * 1, spriteHeight * 2, spriteWidth, spriteHeight)),
		}))},
	{ "crouch", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 2, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "jump", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 3, spriteHeight * 1, spriteWidth, spriteHeight)),
		}))},
	{ "shoot", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.15f, Rectf(spriteWidth * 0, spriteHeight * 4, spriteWidth, spriteHeight)),
			new AnimationFrame(0.15f, Rectf(spriteWidth * 1, spriteHeight * 4, spriteWidth, spriteHeight)),
		}))},
	{ "shootCrouched", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.15f, Rectf(spriteWidth * 2, spriteHeight * 4, spriteWidth, spriteHeight)),
			new AnimationFrame(0.15f, Rectf(spriteWidth * 3, spriteHeight * 4, spriteWidth, spriteHeight)),
		}))},
	{ "climb", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(0.15f, Rectf(spriteWidth * 0, spriteHeight * 3, spriteWidth, spriteHeight)),
			new AnimationFrame(0.15f, Rectf(spriteWidth * 1, spriteHeight * 3, spriteWidth, spriteHeight)),
		}))},
	{ "hurt", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 0, spriteHeight * 5, spriteWidth, spriteHeight)),
		}))},
	};

	const std::list<AnimatorTransition*> playerTransitions
	{
		new ConditionalAnimatorTransition("idle", "crouch", "isCrouched", true),
		new ConditionalAnimatorTransition("idle", "shoot", "isShooting", true),
		new ConditionalAnimatorTransition("idle", "jump", "isGrounded", false),
		new ConditionalAnimatorTransition("idle", "walk", "isWalking", true),

		new ConditionalAnimatorTransition("crouch", "idle", "isCrouched", false),
		new ConditionalAnimatorTransition("crouch", "shootCrouched", "isShooting", true),

		new AnimatorAnimationEndTransition("shoot", "idle"),

		new AnimatorAnimationEndTransition("shootCrouched", "crouch"),

		new ConditionalAnimatorTransition("jump", "idle", "isGrounded", true),
		new ConditionalAnimatorTransition("jump", "shoot", "isShooting", true),

		new ConditionalAnimatorTransition("walk", "idle", "isWalking", false),
		new ConditionalAnimatorTransition("walk", "crouch", "isCrouched", true),
		new ConditionalAnimatorTransition("walk", "jump", "isGrounded", false),
		new ConditionalAnimatorTransition("walk", "shoot", "isShooting", true),

		new ConditionalAnimatorTransition("idle", "climb", "isClimbing", true),
		new ConditionalAnimatorTransition("crouch", "climb", "isClimbing", true),
		new ConditionalAnimatorTransition("jump", "climb", "isClimbing", true),
		new ConditionalAnimatorTransition("walk", "climb", "isClimbing", true),

		new ConditionalAnimatorTransition("climb", "idle", "isClimbing", false),

		new ConditionalAnimatorTransition("idle", "hurt", "isHurt", true),
		new ConditionalAnimatorTransition("crouch", "hurt", "isHurt", true),
		new ConditionalAnimatorTransition("shoot", "hurt", "isHurt", true),
		new ConditionalAnimatorTransition("shootCrouched", "hurt", "isHurt", true),
		new ConditionalAnimatorTransition("jump", "hurt", "isHurt", true),
		new ConditionalAnimatorTransition("walk", "hurt", "isHurt", true),

		new ConditionalAnimatorTransition("hurt", "idle", "isHurt", false),
	};

	m_pPlayer->AddComponent(new AnimatorRenderer(
		m_pPlayer,
		m_pTextureCache->GetTexture("player"),
		playerStates,
		playerTransitions,
		"idle"
	));


	// PHYSICS
	m_pPlayer->AddComponent(new PlayerCollider(m_pPlayer));
	m_pPlayer->AddComponent(new PhysicsBody(m_pPlayer));

	// LOGIC
	m_pPlayer->AddComponent(new PlayerController(m_pPlayer));

	m_pPlayer->Initialize();
}

void LevelScene::CreateLevel()
{
	Texture* foreground{ m_pTextureCache->LoadTexture("level1Foreground", "level1Foreground.png") };
	Texture* background{ m_pTextureCache->LoadTexture("level1Background", "level1Background.png") };

	m_LevelSize = Vector2f(foreground->GetWidth(), foreground->GetHeight());

	Entity* pBackground{ GetEntityKeeper()->CreateEntity(-10) };

	m_pBackgroundTransform = new Transform(pBackground, Vector2f(m_LevelSize.x / 2, 0));
	pBackground->AddComponent(m_pBackgroundTransform);
	pBackground->AddComponent(new Renderer(pBackground, background));

	pBackground->Initialize();


	Entity* pForeground{ GetEntityKeeper()->CreateEntity(-5) };

	pForeground->AddComponent(new Transform(pForeground, Vector2f(0, 0)));
	pForeground->AddComponent(new Renderer(pForeground, foreground));

	// Hardcoded values for map collision TODO: json-ify
	pForeground->AddComponent(new Collider(pForeground, std::vector<Vector2f>{
		Vector2f(0, 0),
			Vector2f(0, 40),
			Vector2f(2000, 40),
			Vector2f(2000, 0),
	}));
	pForeground->AddComponent(new Collider(pForeground, std::vector<Vector2f>{
		Vector2f(610 + 492, 110),
			Vector2f(610 + 492, 110 + 10),
			Vector2f(610, 110 + 10),
			Vector2f(610, 110),
	}));
	pForeground->AddComponent(new Collider(pForeground, std::vector<Vector2f>{
		Vector2f(-5, 0),
			Vector2f(-5, 200),
			Vector2f(0, 200),
			Vector2f(0, 0),
	}));

	pForeground->Initialize();

	CreateLadder(727);
	CreateLadder(919);
	CreateLadder(1075);
}

void LevelScene::CreateLadder(float xCoord) const
{
	Entity* pLadder1{ m_pEntityKeeper->CreateEntity(0, "Ladder") };

	const float ladderWidth{ 15.f };
	const float ladderHeight{ 104.f };

	pLadder1->AddComponent(new Transform(pLadder1, Vector2f(xCoord, 92)));
	pLadder1->AddComponent(new LadderCollider(pLadder1, std::vector<Vector2f>{
		Vector2f(-ladderWidth / 2, -ladderHeight / 2),
			Vector2f(-ladderWidth / 2, ladderHeight / 2),
			Vector2f(ladderWidth / 2, ladderHeight / 2),
			Vector2f(ladderWidth / 2, -ladderHeight / 2),
	}));

	pLadder1->Initialize();
}

void LevelScene::CreateZombieSpawner(Vector2f pos) const
{
	Entity* pSpawner1{ GetEntityKeeper()->CreateEntity() };
	pSpawner1->AddComponent(new Transform(pSpawner1, pos));
	pSpawner1->AddComponent(new EnemySpawner(pSpawner1, this, SpawnerType::random, EnemyType::zombie));
}
