#pragma once

class SoundHandler;
class TextureCache;
class InputHandler;
class EntityKeeper;
class Entity;
class PhysicsHandler;

class Component
{
public:
	explicit Component(Entity* parent);
	Component& operator=(const Component& rhs) = delete;
	Component& operator=(Component&& rhs) = delete;
	Component(const Component& rhs) = delete;
	Component(Component&& rhs) = delete;
	virtual ~Component() = default;

	Entity* GetParent() const;

	virtual void Initialize() {}
	virtual void Update(float deltaTime) {}
	virtual void Draw() const {}

	// Shortcuts for use in components
	EntityKeeper* GetEntityKeeper() const;
	PhysicsHandler* GetPhysicsHandler() const;
	InputHandler* GetInputHandler() const;
	TextureCache* GetTextureCache() const;
	SoundHandler* GetSoundHandler() const;
protected:
	Entity* m_pParent;
};