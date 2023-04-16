#include "base.h"
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
        DPRINTF("PlayerCollider::OnCollisionEnter: Ladder\n");
		m_pTouchedLadder = dynamic_cast<LadderCollider*>(other);
	} else if(other->CompareTag("foreground"))
	{
        DPRINTF("PlayerCollider::OnCollisionEnter: foreground\n");
	} else if(other->CompareTag("gravestone"))
    {
        m_pPlayerController->Event("gravestone:on");
    }
}

void PlayerCollider::OnCollisionUpdate(Collider* other, float deltaTime)
{
	if(other->CompareTag("Ladder")) {
        DPRINTF("PlayerCollider::OnCollisionUpdate: Ladder\n");
        m_pPlayerController->Event("Ladder");
    }

	if(other->CompareTag("foreground"))
	{
        DPRINTF("PlayerCollider::OnCollisionUpdate: foreground\n");
        m_pPlayerController->Event("grounded:on");
	}
}

void PlayerCollider::OnCollisionExit(Collider* other, float deltaTime)
{
	if (other->CompareTag("Ladder"))
	{
        DPRINTF("PlayerCollider::OnCollisionExit: Ladder\n");
		m_pTouchedLadder = nullptr;
	}
	if(other->CompareTag("foreground"))
	{
        DPRINTF("PlayerCollider::OnCollisionExit: foreground\n");
		//m_pPlayerController->m_IsGrounded = false;
        m_pPlayerController->Event("grounded:off");
	}
	if(other->CompareTag("gravestone"))
    {
        m_pPlayerController->Event("gravestone:off");
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
