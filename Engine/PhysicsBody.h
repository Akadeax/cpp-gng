#pragma once
#include <list>
#include "Component.h"
#include "Vector2f.h"

class Transform;
class Collider;
class PhysicsHandler;

class PhysicsBody final : public Component
{
public:
	explicit PhysicsBody(Entity* parent);
	PhysicsBody& operator=(const PhysicsBody& rhs) = delete;
	PhysicsBody& operator=(PhysicsBody&& rhs) = delete;
	PhysicsBody(const PhysicsBody& rhs) = delete;
	PhysicsBody(PhysicsBody&& rhs) = delete;
	~PhysicsBody() override;

	void Initialize() override;

	Transform* GetTransform() const;
	Collider* GetCollider() const;

	const Vector2f& GetVelocity() const;
	void SetVelocity(const Vector2f& velocity);
	void SetXVelocity(float x);
	void SetYVelocity(float y);
	void AddVelocity(const Vector2f& velocity);

private:
	Transform* m_pTransform{ nullptr };
	Collider* m_pCollider{ nullptr };

	Vector2f m_Velocity{ Vector2f() };
};