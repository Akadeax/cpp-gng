#include "base.h"
#include "pch.h"
#include "LadderCollider.h"

#include <cassert>

#include "Entity.h"
#include "PhysicsHandler.h"
#include "Scene.h"
#include "Transform.h"

LadderCollider::LadderCollider(Entity* pParent, const std::vector<Vector2f>& vertices)
	: Collider(pParent, vertices, true)
{
	assert(vertices.size() == 4 && "Ladder collider does not have 4 vertices.");
}

bool LadderCollider::IsPlayerAtBottom() const
{
    Vector2f vL1 = m_TransformedVertices[0] + Vector2f(0, 0),
             vL2 = m_TransformedVertices[3] + Vector2f(0, 1);

    DPRINTF("IsPlayerAtBottom: vL1 = %s, vL2 = %s\n", vL1.ToString().c_str(), vL2.ToString().c_str());

	const PhysicsHandler* physics{ m_pParent->GetScene()->GetPhysicsHandler() };
	return physics->Linecast(
		m_TransformedVertices[0] + Vector2f(0, 1),
		m_TransformedVertices[3] + Vector2f(0, 1),
		"Player",
		true
	).first;
}

bool LadderCollider::IsPlayerAtTop() const
{
    Vector2f vL1 = m_TransformedVertices[1] + Vector2f(-5, 0),
             vL2 = m_TransformedVertices[2] + Vector2f(0, 5);

    DPRINTF("IsPlayerAtTop: vL1 = %s, vL2 = %s\n", vL1.ToString().c_str(), vL2.ToString().c_str());

	const PhysicsHandler* physics{ m_pParent->GetScene()->GetPhysicsHandler() };
    //std::pair<bool, Collider*> collisions = physics->Linecast(
    return physics->Linecast(
		m_TransformedVertices[1] - Vector2f(0, 1),
		m_TransformedVertices[2] - Vector2f(0, 1),
		"Player",
		true
	).first;

// Debugging code
/*
    if(collisions.first == true) {
        printf("IsPlayerAtTop: collision %s name=%s\n", 
            collisions.first?"TRUE":"FALSE",
            collisions.second->GetTag().c_str());
        return collisions.first;
    } else {
        printf("IsPlayerAtTop: NOOO collision \n"); 
        return false;
    }
*/
}

float LadderCollider::GetLadderX() const
{
	return m_pTransform->GetPosition().x;
}
