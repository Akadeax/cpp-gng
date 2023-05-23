// ReSharper disable once CppUnusedIncludeDirective, This is pch
#include "base.h"

#include "Camera.h"
#include <SDL_opengl.h>

Camera::Camera(float initialScale)
	: m_Scale{ initialScale }
{
}

const Vector2f& Camera::GetPosition() const
{
	return m_Position;
}

void Camera::SetPosition(const Vector2f& newPos)
{
	m_Position = newPos;
}

void Camera::MovePosition(const Vector2f& moveBy)
{
	m_Position += moveBy;
}

void Camera::Update(float deltaTime)
{
}

void Camera::Draw() const
{
	glScalef(m_Scale, m_Scale, 0);

	glTranslatef(-m_Position.x, -m_Position.y, 0);
}

