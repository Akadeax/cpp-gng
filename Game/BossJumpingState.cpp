#include "pch.h"
#include "BossJumpingState.h"

#include <cassert>

#include "AnimatorRenderer.h"
#include "Entity.h"
#include "PhysicsBody.h"
#include "Transform.h"

BossJumpingState::BossJumpingState(Animation* anim)
	: AnimatorState(anim)
{
}

void BossJumpingState::OnStateEnter(AnimatorRenderer* animator)
{
	if (m_pTransform == nullptr)
	{
		m_pTransform = animator->GetParent()->GetComponent<Transform>();
		assert(m_pTransform != nullptr && "Entity has Boss component but no Transform component");
	}
	if (m_pPhysicsBody == nullptr)
	{
		m_pPhysicsBody = animator->GetParent()->GetComponent<PhysicsBody>();
		assert(m_pPhysicsBody != nullptr && "Entity has Boss component but no PhysicsBody component");
	}

	m_StartingHeight = m_pTransform->GetPosition().y;
	m_pTransform->MovePosition(Vector2f(0, 1));

	m_pPhysicsBody->SetYVelocity(m_JumpStrength);
}

void BossJumpingState::OnStateUpdate(AnimatorRenderer* animator, float deltaTime)
{
	m_pPhysicsBody->SetXVelocity(0);

	if (m_pTransform->GetPosition().y <= m_StartingHeight)
	{
		m_pTransform->SetYPosition(m_StartingHeight);
		animator->SetState("walk");
	}

	m_pPhysicsBody->AddVelocity(Vector2f(0, -m_GravityStrength * deltaTime));
}

void BossJumpingState::OnStateExit(AnimatorRenderer* animator)
{
	m_pPhysicsBody->SetYVelocity(0);

}
