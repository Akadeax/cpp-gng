#include "base.h"
#include "Transform.h"

Transform::Transform(Entity* parent)
	: Transform(parent, Vector2f{ 0.f, 0.f })
{
}

Transform::Transform(Entity* parent, const Vector2f& startingPos)
	: Component(parent)
	, m_Position{ startingPos }
	, m_RotationDeg{ 0.f }
	, m_Scale{ 1.f }
{
}

const Vector2f& Transform::GetPosition() const
{
	return m_Position;
}

void Transform::SetPosition(const Vector2f& newPos)
{
	m_Position = newPos;
}

void Transform::MovePosition(const Vector2f& delta)
{
	m_Position += delta;
}

void Transform::SetXPosition(float newX)
{
	m_Position.x = newX;
}

void Transform::SetYPosition(float newY)
{
	m_Position.y = newY;
}

float Transform::GetRotation() const
{
	return m_RotationDeg;
}

void Transform::SetRotation(float newRotation)
{
	m_RotationDeg = newRotation;
}

void Transform::Rotate(float delta)
{
	m_RotationDeg += delta;
}

float Transform::GetScale() const
{
	return m_Scale;
}

void Transform::SetScale(float newScale)
{
	m_Scale = newScale;
}
