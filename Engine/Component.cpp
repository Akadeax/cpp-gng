#include "base.h"

#include "Component.h"

#include "Entity.h"
#include "Game.h"
#include "Scene.h"

Component::Component(Entity* parent)
	: m_pParent{ parent }
{
}

Entity* Component::GetParent() const
{
	return m_pParent;
}

EntityKeeper* Component::GetEntityKeeper() const
{
	return m_pParent->GetScene()->GetEntityKeeper();
}

PhysicsHandler* Component::GetPhysicsHandler() const
{
	return m_pParent->GetScene()->GetPhysicsHandler();
}

InputHandler* Component::GetInputHandler() const
{
	return m_pParent->GetScene()->GetGame()->GetInputHandler();
}

TextureCache* Component::GetTextureCache() const
{
	return m_pParent->GetScene()->GetTextureCache();
}

SoundHandler* Component::GetSoundHandler() const
{
	return m_pParent->GetScene()->GetGame()->GetSoundHandler();
}
