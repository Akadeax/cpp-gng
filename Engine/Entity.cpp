#include "base.h"

#include "Entity.h"
#include "Scene.h"

void Entity::AddComponent(Component* comp)
{
	m_Components.push_back(comp);
}

std::vector<Component*> Entity::GetComponents() const
{
	return m_Components;
}

void Entity::Initialize() const
{
    printf("Entity:Initialize %s\n", m_Tag.c_str());
	for (Component* component : m_Components)
	{
		component->Initialize();
	}
}

void Entity::SetTag(const std::string& newTag)
{
	m_Tag = newTag;
}

const std::string& Entity::GetTag() const
{
	return m_Tag;
}

void Entity::SetActive(bool newState)
{
	m_IsActive = newState;
}

bool Entity::IsActive() const
{
	return m_IsActive;
}

Scene* Entity::GetScene() const
{
	return m_pScene;
}

int Entity::GetUpdatePriority() const
{
	return m_UpdatePriority;
}

Entity::Entity(Scene* scene, int updatePriority)
	: m_pScene{ scene }, m_UpdatePriority{ updatePriority }
{
}

Entity::~Entity()
{
	for(size_t i{ 0 }; i < m_Components.size(); ++i)
	{
		delete m_Components[i];
		m_Components[i] = nullptr;
	}
}
