#include "base.h"

#include <cassert>

//#include "PhysicsHandler.h"
#include "Collider.h"
#include "utils.h"
#include "Entity.h"
#include "PhysicsHandler.h"
#include "Transform.h"

Collider::Collider(Entity* pParent, std::vector<Vector2f> vertices, bool isTrigger)
	: Component(pParent)
	, m_IsTrigger{ isTrigger }
{
	assert((vertices.size() > 2 || vertices.empty()) && "Not enough vertices given for collider");

	m_BaseVertices = std::move(vertices);
	m_TransformedVertices = std::vector<Vector2f>(m_BaseVertices.size());
}

Collider::~Collider()
{
	GetPhysicsHandler()->RemoveCollider(this);
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

std::string Collider::GetTag() const
{
	return m_pParent->GetTag();
}
bool Collider::CompareTag(const std::string& tag) const
{
	return m_pParent->GetTag() == tag;
}

std::vector<Vector2f>& Collider::GetBaseVertices()
{
	return m_BaseVertices;
}

void Collider::SetBaseVertices(std::vector<Vector2f> newVertices)
{
	m_BaseVertices = std::move(newVertices);
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
