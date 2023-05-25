#include "pch.h"
#include "MovingPlatform.h"

#include "Entity.h"
#include "Transform.h"

MovingPlatform::MovingPlatform(Entity* pParent, const std::vector<Vector2f>& vertices, float xStart, float xEnd)
	: Collider(pParent, vertices, false)
	, m_XStart{ xStart }
	, m_XEnd{ xEnd }
{
}

void MovingPlatform::Initialize()
{
	Collider::Initialize();
	m_YPos = m_pTransform->GetPosition().y;
}

void MovingPlatform::Update(float deltaTime)
{
	this->RecalculateTransformedVertices();

	m_Time += deltaTime;
	const float intervalSize{ m_XEnd - m_XStart };
	const float newX{ (intervalSize / 2.f) * sinf(m_Time) + m_XStart + (intervalSize / 2.f) };

	if (m_pCollidingPlayer != nullptr)
	{
		m_pCollidingPlayer->MovePosition(Vector2f(newX - m_pTransform->GetPosition().x, 0));
	}

	m_pTransform->SetPosition(Vector2f(newX, m_YPos));
}

void MovingPlatform::OnCollisionEnter(Collider* other, float deltaTime)
{
	if(other->CompareTag("Player"))
	{
		m_pCollidingPlayer = other->GetParent()->GetComponent<Transform>();
	}
}

void MovingPlatform::OnCollisionExit(Collider* other, float deltaTime)
{
	if (other->CompareTag("Player"))
	{
		m_pCollidingPlayer = nullptr;
	}
}
