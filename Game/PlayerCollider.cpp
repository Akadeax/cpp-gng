#include "pch.h"
#include "PlayerCollider.h"

#include <cassert>

#include "Entity.h"
#include "LadderCollider.h"
#include "PlayerController.h"
#include "Transform.h"

PlayerCollider::PlayerCollider(Entity* pParent)
	: Collider(pParent, std::vector<Vector2f>{})
{
}

void PlayerCollider::Initialize()
{
	Collider::Initialize();

	m_pPlayerController = m_pParent->GetComponent<PlayerController>();
	assert(m_pPlayerController != nullptr && "Entity has PlayerCollider component but no PlayerController component");
}

void PlayerCollider::OnCollisionEnter(Collider* other, float deltaTime)
{
	if (other->CompareTag("Ladder"))
	{
		m_pTouchedLadder = dynamic_cast<LadderCollider*>(other);
	}
	else if (other->CompareTag("Enemy") && m_CanInteract)
	{
		m_pPlayerController->Damage(other->GetTransform()->GetPosition());
	}
}

void PlayerCollider::OnCollisionExit(Collider* other, float deltaTime)
{
	if (other->CompareTag("Ladder"))
	{
		m_pTouchedLadder = nullptr;
	}
}

bool PlayerCollider::IsTouchingLadder() const
{
	return m_pTouchedLadder != nullptr;
}

LadderCollider* PlayerCollider::GetTouchedLadder() const
{
	return m_pTouchedLadder;
}

void PlayerCollider::SetCanInteract(bool newVal)
{
	m_CanInteract = newVal;
}
