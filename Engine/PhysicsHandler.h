#pragma once
#include <list>
#include <map>
#include "Vector2f.h"

class Collider;
class PhysicsBody;

class PhysicsHandler final
{
public:
	explicit PhysicsHandler() = default;
	PhysicsHandler& operator=(const PhysicsHandler& rhs) = delete;
	PhysicsHandler& operator=(PhysicsHandler&& rhs) = delete;
	PhysicsHandler(const PhysicsHandler& rhs) = delete;
	PhysicsHandler(PhysicsHandler&& rhs) = delete;
	~PhysicsHandler() = default;

	void Update(float deltaTime);
	void DrawDebugColliders() const;

	void AddCollider(Collider* collider);
	void AddPhysicsBody(PhysicsBody* physicsBody);

	void RemoveCollider(Collider* collider);
	void RemovePhysicsBody(PhysicsBody* physicsBody);

	std::pair<bool, Collider*> Linecast(const Vector2f& p1, const Vector2f& p2, const std::string& tag = "", bool hitTriggers = false) const;

private:
	std::list<Collider*> m_Colliders;
	std::list<PhysicsBody*> m_PhysicsBodies;

	std::map<std::pair<PhysicsBody*, Collider*>, bool> m_LastFrameCollisions;

	void ResolveCollisions(float deltaTime) const;
	void NotifyColliders(float deltaTime);
};

