#include "base.h"
#include "Scene.h"

#include <cassert>
#include <iostream>

#include "Camera.h"
#include "EntityKeeper.h"
#include "PhysicsHandler.h"
#include "TextureCache.h"

Scene::Scene()
	: Scene(nullptr, "")
{
}

Scene::Scene(Camera* pCamera, std::string ResourcesFolder)
	: m_pGame{ nullptr }
	, m_pEntityKeeper{ new EntityKeeper(this) }
	, m_pPhysicsHandler{ new PhysicsHandler() }
	, m_pTextureCache{ new TextureCache(ResourcesFolder) }
	, m_pCamera{ pCamera }
{
}

Scene::~Scene()
{
	delete m_pEntityKeeper;
	delete m_pPhysicsHandler;
	delete m_pTextureCache;
	delete m_pCamera;
}

void Scene::Initialize(Game* pGame)
{
	m_pGame = pGame;

	InitializeScene();
	assert(m_pCamera != nullptr && "Scene has no assigned camera");
}

void Scene::Update(float deltaTime)
{
	m_pPhysicsHandler->Update(deltaTime);
	m_pEntityKeeper->UpdateEntities(deltaTime);
	m_pCamera->Update(deltaTime);

	UpdateScene(deltaTime);
}

void Scene::Draw() const
{
	// Draw UI above here
	glPushMatrix();
	m_pCamera->Draw();
	m_pEntityKeeper->DrawEntities();
	DrawScene();
	glPopMatrix();
}

Game* Scene::GetGame() const
{
	return m_pGame;
}

EntityKeeper* Scene::GetEntityKeeper() const
{
	return m_pEntityKeeper;
}

PhysicsHandler* Scene::GetPhysicsHandler() const
{
	return m_pPhysicsHandler;
}

TextureCache* Scene::GetTextureCache() const
{
	return m_pTextureCache;
}
