#include "pch.h"
#include "PlayerCamera.h"

#include <iostream>

#include "Collider.h"
#include "constants.h"
#include "LevelScene.h"
#include "Transform.h"

PlayerCamera::PlayerCamera(LevelScene* pLevelScene, Transform* followTarget)
	: Camera(3.1f)
	, m_pLevelScene{ pLevelScene }
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

	if (newCamPos.x >= m_EndStartPos && !m_IsAtEnd)
	{
		m_IsAtEnd = true;
		m_LeftBoundary = m_EndLeftBoundary;
		m_pLevelScene->AddForegroundCollider(Vector2f(m_EndStartPos - 5, 0), Vector2f(5, 300));
	}

	SetPosition(newCamPos);
}

void PlayerCamera::SetFollowTarget(Transform* target)
{
	m_pFollowTarget = target;
}
