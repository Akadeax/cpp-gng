#include "pch.h"
#include "LevelScene.h"

#include "Animation.h"
#include "AnimationFrame.h"
#include "AnimationEndAnimatorTransition.h"
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
#include "InputHandler.h"
#include "LadderCollider.h"
#include "MovingPlatform.h"
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
	CreatePlayer();

	m_pProjectilePool = new ProjectilePool(this);
	m_pSpawnerKeeper = new SpawnerKeeper(this);

	m_pCamera = new PlayerCamera(this, m_pPlayer->GetComponent<Transform>());
	// Center camera at start
	m_pCamera->MovePosition(Vector2f(-450, -300));

	CreateLevel();

	const float spawnerDistFromBorder{ 20.f };
	const int spawnerAmount{ 20 };
	const float spawnerIntervalDist{ 60.f };
	const float spawnerHeight{ 40.f };
	for(int i{}; i < spawnerAmount; ++i)
	{
		CreateZombieSpawner(Vector2f(spawnerDistFromBorder + spawnerIntervalDist * static_cast<float>(i), spawnerHeight));
	}

	CreateSetSpawner(Vector2f(360, spawnerHeight), EnemyType::zombie);
	CreateSetSpawner(Vector2f(560, spawnerHeight), EnemyType::zombie);


	CreateSetSpawner(Vector2f(800, 119), EnemyType::greenMonster);


	CreateSetSpawner(Vector2f(2840, spawnerHeight), EnemyType::greenMonster);
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

	if (m_pGame->GetInputHandler()->GetKeyDown("information"))
	{
		std::cout << "Controls:\nArrows keys: move\nJ: jump\nK: shoot\n";
	}
}

void LevelScene::DrawScene() const
{
#ifdef _DEBUG
	m_pPhysicsHandler->DrawDebugColliders();
	m_pSpawnerKeeper->DrawDebugRadius();
#endif
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

void LevelScene::AddForegroundCollider(Vector2f bottomLeft, Vector2f size) const
{
	m_pForeground->AddComponentInitialized(Collider::FromBottomLeft(m_pForeground, bottomLeft, size));
}

void LevelScene::CreatePlayer()
{
	m_pTextureCache->LoadTexture("player", "player_22x25.png");
	m_pTextureCache->LoadTexture("playerNaked", "playerNaked_22x25.png");

	m_pPlayer = m_pEntityKeeper->CreateEntity(100, "Player");
	//m_pPlayer->AddComponent(new Transform(m_pPlayer, Vector2f(100, 40)));
	m_pPlayer->AddComponent(new Transform(m_pPlayer, Vector2f(2600, 40)));

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
	{ "deadAir", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 1, spriteHeight * 5, spriteWidth, spriteHeight)),
		}))},
	{ "deadGround", new AnimatorState(new Animation(std::vector<AnimationFrame*>{
			new AnimationFrame(1.f, Rectf(spriteWidth * 2, spriteHeight * 5, spriteWidth, spriteHeight)),
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

		new AnimationEndAnimatorTransition("shoot", "idle"),

		new AnimationEndAnimatorTransition("shootCrouched", "crouch"),

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
	pBackground->AddComponent(new Renderer(pBackground, background, false));

	pBackground->Initialize();


	m_pForeground = GetEntityKeeper()->CreateEntity(-5);

	m_pForeground->AddComponent(new Transform(m_pForeground, Vector2f(0, 0)));
	m_pForeground->AddComponent(new Renderer(m_pForeground, foreground, false));

	// Map collision
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(-5, 0), Vector2f(5, 200)));

	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(0, 0), Vector2f(1662, 40)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(610, 111), Vector2f(494, 9)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(1791, 0), Vector2f(159, 40)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(1983, 0), Vector2f(32, 40)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(2047, 0), Vector2f(400, 40)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(2478, 0), Vector2f(527, 40)));

	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(3005, 0), Vector2f(5, 200)));

	// Graves
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(47, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(239, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(415, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(527, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(751, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(959, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(1103, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(1263, 40), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(1519, 40), Vector2f(16, 16)));

	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(767, 120), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(863, 120), Vector2f(16, 16)));
	m_pForeground->AddComponent(Collider::FromBottomLeft(m_pForeground, Vector2f(959, 120), Vector2f(16, 16)));
	
	m_pForeground->Initialize();


	Texture* pMovingPlatformTexture{ GetTextureCache()->LoadTexture("movingPlatform", "movingPlatform.png")};

	Entity* pMovingPlatform1{ GetEntityKeeper()->CreateEntity(5) };

	pMovingPlatform1->AddComponent(new Transform(pMovingPlatform1, Vector2f(1688, 25)));

	const Vector2f movingPlatformSize{ Vector2f(42.f, 13.f) };
	pMovingPlatform1->AddComponent(new MovingPlatform(
		pMovingPlatform1,
		std::vector<Vector2f>{
			Vector2f(-movingPlatformSize.x / 2.f, -movingPlatformSize.y / 2.f),
			Vector2f(-movingPlatformSize.x / 2.f, movingPlatformSize.y / 2.f),
			Vector2f(movingPlatformSize.x / 2.f, movingPlatformSize.y / 2.f),
			Vector2f(movingPlatformSize.x / 2.f, -movingPlatformSize.y / 2.f),
		},
		1688, 1755
	));
	pMovingPlatform1->AddComponent(new Renderer(pMovingPlatform1, pMovingPlatformTexture));

	pMovingPlatform1->Initialize();

	CreateLadder(727);
	CreateLadder(919);
	CreateLadder(1078);
}

void LevelScene::CreateLadder(float xCoord) const
{
	Entity* pLadder{ m_pEntityKeeper->CreateEntity(0, "Ladder") };

	const float ladderWidth{ 15.f };
	const float ladderHeight{ 104.f };

	pLadder->AddComponent(new Transform(pLadder, Vector2f(xCoord, 92)));
	pLadder->AddComponent(new LadderCollider(pLadder, std::vector<Vector2f>{
		Vector2f(-ladderWidth / 2, -ladderHeight / 2),
			Vector2f(-ladderWidth / 2, ladderHeight / 2),
			Vector2f(ladderWidth / 2, ladderHeight / 2),
			Vector2f(ladderWidth / 2, -ladderHeight / 2),
	}));

	pLadder->Initialize();
}

void LevelScene::CreateZombieSpawner(const Vector2f& pos) const
{
	Entity* pSpawner{ GetEntityKeeper()->CreateEntity() };
	pSpawner->AddComponent(new Transform(pSpawner, pos));
	pSpawner->AddComponent(new EnemySpawner(pSpawner, this, SpawnerType::random, EnemyType::zombie));
}

void LevelScene::CreateSetSpawner(const Vector2f& pos, EnemyType type) const
{
	Entity* pSpawner{ GetEntityKeeper()->CreateEntity() };
	pSpawner->AddComponent(new Transform(pSpawner, pos));
	pSpawner->AddComponent(new EnemySpawner(pSpawner, this, SpawnerType::set, type));
}
