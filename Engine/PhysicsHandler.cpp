#include "base.h"

#include <iostream>

#include "PhysicsHandler.h"
#include "Transform.h"
#include "Collider.h"
#include "PhysicsBody.h"
#include "collisions.h"
#include "Entity.h"

void PhysicsHandler::Update(float deltaTime)
{
	ResolveCollisions(deltaTime);

	// Apply Velocity
	for (const PhysicsBody* currentPhysicsBody : m_PhysicsBodies)
	{
		currentPhysicsBody->GetTransform()->MovePosition(currentPhysicsBody->GetVelocity() * deltaTime);
	}

	NotifyColliders(deltaTime);

}

#ifdef _DEBUG
void PhysicsHandler::DrawDebugColliders() const
{
	for(const Collider* pCollider : m_Colliders)
	{
		if (!pCollider->GetParent()->IsActive()) continue;

		pCollider->DrawDebugCollider();
	}
}
#endif

void PhysicsHandler::AddCollider(Collider* collider)
{
	m_Colliders.push_back(collider);
}

void PhysicsHandler::AddPhysicsBody(PhysicsBody* physicsBody)
{
	m_PhysicsBodies.push_back(physicsBody);
}

void PhysicsHandler::RemoveCollider(Collider* collider)
{
	m_Colliders.remove(collider);
}

void PhysicsHandler::RemovePhysicsBody(PhysicsBody* physicsBody)
{
	m_PhysicsBodies.remove(physicsBody);
}

std::pair<bool, Collider*> PhysicsHandler::Linecast(const Vector2f& p1, const Vector2f& p2, const std::string& tag, bool hitTriggers) const
{
	for (Collider* currentCollider : m_Colliders)
	{
		if (!hitTriggers && currentCollider->IsTrigger()) continue;

		if (collisions::IntersectLinePolygon(p1, p2, currentCollider->GetTransformedVertices()))
		{
			// Check if tag of hit object is correct
			// or we're not checking for tag
			if (tag.empty() || currentCollider->CompareTag(tag))
			{
				return std::make_pair(true, currentCollider);
			}
		}
	}
	return std::make_pair(false, nullptr);
}

void PhysicsHandler::ResolveCollisions(float deltaTime) const
{
	for (const PhysicsBody* physicsBody : m_PhysicsBodies)
	{
		// only physics bodies should be moving, so only recalculate their positions
		physicsBody->GetCollider()->RecalculateTransformedVertices();

		if (physicsBody->GetCollider()->IsTrigger()) continue;

		for (Collider* collider : m_Colliders)
		{
			if (physicsBody->GetCollider()->GetParent() == collider->GetParent()) continue;
			if (collider->IsTrigger() || !collider->GetParent()->IsActive() || !collider->IsEnabled()) continue;

			collisions::CollisionHitInfo currentCollision = collisions::IntersectPolygons(
				physicsBody->GetCollider()->GetTransformedVertices(),
				collider->GetTransformedVertices()
			);

			if (currentCollision.hit)
			{
				if (physicsBody->GetCollider()->IsTrigger() || collider->IsTrigger()) continue;

				// resolve collision by just pushing position back with normal
				physicsBody->GetTransform()->MovePosition(-currentCollision.normal * currentCollision.depth);
			}
		}
	}
}

void PhysicsHandler::NotifyColliders(float deltaTime)
{
	for (PhysicsBody* physicsBody : m_PhysicsBodies)
	{
		if (!physicsBody->GetParent()->IsActive()) continue;

		for (Collider* collider : m_Colliders)
		{
			if (!collider->GetParent()->IsActive() || !collider->IsEnabled()) continue;

			if (physicsBody->GetCollider()->GetParent() == collider->GetParent()) continue;

			std::pair<PhysicsBody*, Collider*> currentCollisionPair{ std::make_pair(physicsBody, collider) };

			// Find current collision state
			const collisions::CollisionHitInfo currentCollision{ collisions::IntersectPolygons(
				physicsBody->GetCollider()->GetTransformedVertices(),
				collider->GetTransformedVertices()
			) };

			const bool currentlyColliding{ currentCollision.hit };
			const bool collidedLastFrame{ m_LastFrameCollisions[currentCollisionPair] };

			// Check collision state (against set data from last frame)
			if (!collidedLastFrame && currentlyColliding)
			{
				physicsBody->GetCollider()->OnCollisionEnter(collider, deltaTime);
				collider->OnCollisionEnter(physicsBody->GetCollider(), deltaTime);
			}
			else if (collidedLastFrame && currentlyColliding)
			{
				physicsBody->GetCollider()->OnCollisionUpdate(collider, deltaTime);
				collider->OnCollisionUpdate(physicsBody->GetCollider(), deltaTime);
			}
			else if (collidedLastFrame && !currentlyColliding)
			{
				physicsBody->GetCollider()->OnCollisionExit(collider, deltaTime);
				collider->OnCollisionExit(physicsBody->GetCollider(), deltaTime);
			}

			// Set collision state data for next frame
			m_LastFrameCollisions[currentCollisionPair] = currentlyColliding;

			// If collided position has changed, recalculate
			if (currentlyColliding)
			{
				physicsBody->GetCollider()->RecalculateTransformedVertices();
			}
		}
	}
}