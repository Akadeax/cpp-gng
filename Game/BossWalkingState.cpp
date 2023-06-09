#include "pch.h"
#include "BossWalkingState.h"

#include <cassert>

#include "AnimatorRenderer.h"
#include "Entity.h"
#include "PhysicsBody.h"

BossWalkingState::BossWalkingState(Animation* anim)
	: AnimatorState(anim)
{
}

void BossWalkingState::OnStateEnter(AnimatorRenderer* animator)
{
	if (m_pPhysicsBody == nullptr)
	{
		m_pPhysicsBody = animator->GetParent()->GetComponent<PhysicsBody>();
		assert(m_pPhysicsBody != nullptr && "Entity has Boss component but no Transform component");
	}

	m_CurrentMoveTime = 0.f;
}

void BossWalkingState::OnStateUpdate(AnimatorRenderer* animator, float deltaTime)
{
	m_CurrentMoveTime += deltaTime;

	if (m_CurrentMoveTime > m_MoveTime)
	{
		if (m_Direction == left)
		{
			animator->SetState("jump");
		}
		else
		{
			animator->SetState("shoot");
		}
	}

	const float dirMultiplier{ m_Direction == left ? -1.f : 1.f };
	m_pPhysicsBody->SetXVelocity(m_MoveSpeed * dirMultiplier);
}

void BossWalkingState::OnStateExit(AnimatorRenderer* animator)
{
	// Switch direction on exit
	if (m_Direction == left) m_Direction = right;
	else m_Direction = left;

	m_pPhysicsBody->SetXVelocity(0);
}
