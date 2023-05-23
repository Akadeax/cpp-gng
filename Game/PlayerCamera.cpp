#include "pch.h"
#include "PlayerCamera.h"

#include "constants.h"
#include "Transform.h"

PlayerCamera::PlayerCamera(Transform* followTarget)
	: Camera(3.1f)
	, m_pFollowTarget{ followTarget }
{
}

void PlayerCamera::Update(float deltaTime)
{
	// Calc actual camera size (factoring in zoom)
	const Vector2f cameraSize{ Vector2f(constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT) / m_Scale };

	const Vector2f targetPos{ m_pFollowTarget->GetPosition() };
	Vector2f newCamPos = Vector2f(
		targetPos.x - cameraSize.x / 2,
		0
	);

	if (newCamPos.x < m_LeftBoundary) newCamPos.x = m_LeftBoundary;
	else if (newCamPos.x > m_RightBoundary) newCamPos.x = m_RightBoundary;

	SetPosition(newCamPos);
}

void PlayerCamera::SetFollowTarget(Transform* target)
{
	m_pFollowTarget = target;
}
