#include "base.h"

#include <cassert>

//#include "PhysicsHandler.h"
#include "Collider.h"
#include "utils.h"
#include "Entity.h"
#include "PhysicsHandler.h"
#include "Transform.h"

Collider::Collider(Entity* pParent, const std::vector<Vector2f>& vertices, bool isTrigger)
	: Component(pParent)
	, m_IsTrigger{ isTrigger }
{
	assert((vertices.size() > 2 || vertices.empty()) && "Not enough vertices given for collider");

	m_BaseVertices = vertices;
	m_TransformedVertices = std::vector<Vector2f>(m_BaseVertices.size());
}

Collider::~Collider()
{
	GetPhysicsHandler()->RemoveCollider(this);
}

Collider* Collider::FromBottomLeft(Entity* pParent, const Vector2f& bottomLeft, const Vector2f& size, bool isTrigger)
{
	return new Collider(pParent, std::vector<Vector2f>{
		bottomLeft,
		Vector2f(bottomLeft.x, bottomLeft.y + size.y),
		Vector2f(bottomLeft.x + size.x, bottomLeft.y + size.y),
		Vector2f(bottomLeft.x + size.x, bottomLeft.y),
	}, isTrigger);
}

Collider* Collider::FromCenter(Entity* pParent, const Vector2f& center, const Vector2f& size, bool isTrigger)
{
	const float halfSizeX{ size.x / 2.f };
	const float halfSizeY{ size.x / 2.f };

	return new Collider(pParent, std::vector<Vector2f>{
		Vector2f(center.x - halfSizeX, center.y - halfSizeY),
		Vector2f(center.x - halfSizeX, center.y + halfSizeY),
		Vector2f(center.x + halfSizeX, center.y + halfSizeY),
		Vector2f(center.x + halfSizeX, center.y - halfSizeY),
	}, isTrigger);
}

void Collider::Initialize()
{
	m_pTransform = m_pParent->GetComponent<Transform>();
	assert(m_pTransform != nullptr && "Entity has collider but no transform");
	GetPhysicsHandler()->AddCollider(this);

	RecalculateTransformedVertices();
}

void Collider::OnCollisionUpdate(Collider* other, float deltaTime) { }
void Collider::OnCollisionEnter(Collider* other, float deltaTime) { }
void Collider::OnCollisionExit(Collider* other, float deltaTime) { }

void Collider::DrawDebugCollider() const
{
	const Color4f debugColor{ m_IsTrigger ? Color4f(0, 1, 0, 1) : Color4f(1, 0, 0, 1) };

	utils::SetColor(debugColor);
	for (size_t i{ 0 }; i < m_BaseVertices.size(); ++i)
	{
		utils::DrawPolygon(m_TransformedVertices, true, 2.f);
	}
}

void Collider::RecalculateTransformedVertices()
{
	if(!IsEnabled())
	{
		m_TransformedVertices = std::vector<Vector2f>();
		return;
	}

	if (m_TransformedVertices.size() != m_BaseVertices.size())
	{
		m_TransformedVertices = std::vector<Vector2f>(m_BaseVertices.size());
	}

	const float rotation{ m_pTransform->GetRotation() * (utils::g_Pi / 180.f) };

	const float rotSin{ std::sin(rotation) };
	const float rotCos{ std::cos(rotation) };

	// Transform all vertices to actual world position
	for (size_t i{ 0 }; i < m_BaseVertices.size(); ++i)
	{
		const Vector2f base{ m_BaseVertices[i] };

		// Scale
		const Vector2f scaled{ base * m_pTransform->GetScale() };
		// Rotate
		float newX{ rotCos * scaled.x - rotSin * scaled.y };
		float newY{ rotSin * scaled.x + rotCos * scaled.y };
		// Translate
		newX += m_pTransform->GetPosition().x;
		newY += m_pTransform->GetPosition().y;

		m_TransformedVertices[i] = Vector2f(newX, newY);
	}
}


bool Collider::IsTrigger() const
{
	return m_IsTrigger;
}

void Collider::SetTrigger(bool newVal)
{
	m_IsTrigger = newVal;
}

bool Collider::CompareTag(const std::string& tag) const
{
	return m_pParent->GetTag() == tag;
}

std::vector<Vector2f>& Collider::GetBaseVertices()
{
	return m_BaseVertices;
}

void Collider::SetBaseVertices(const std::vector<Vector2f>& newVertices)
{
	m_BaseVertices = newVertices;
	m_TransformedVertices = std::vector<Vector2f>(m_BaseVertices.size());
}

const std::vector<Vector2f>& Collider::GetTransformedVertices()
{
	return m_TransformedVertices;
}

Transform* Collider::GetTransform() const
{
	return m_pTransform;
}

void Collider::SetEnabled(bool value)
{
	m_IsEnabled = value;

	if (value == true) RecalculateTransformedVertices();
}

bool Collider::IsEnabled() const
{
	return m_IsEnabled;
}

float Collider::GetHeight() const
{
	if (m_BaseVertices.size() <= 1) return 0.f;

	float min{ FLT_MAX };
	float max{ FLT_MIN };

	for(size_t i{}; i < m_BaseVertices.size(); ++i)
	{
		const float currentHeight{ m_BaseVertices[i].y };
		if (currentHeight < min) min = currentHeight;
		if (currentHeight > max) max = currentHeight;
	}

	return max - min;
}
