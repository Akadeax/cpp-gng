#pragma once
#include <Camera.h>

class LevelScene;
class Transform;

class PlayerCamera final :
    public Camera
{
public:
    PlayerCamera(LevelScene* pLevelScene, Transform* followTarget);

    void Update(float deltaTime) override;

    void SetFollowTarget(Transform* target);

private:
    LevelScene* m_pLevelScene;
    Transform* m_pFollowTarget;

    float m_LeftBoundary{ 0.f };
    float m_RightBoundary{ 2715.f };

    bool m_IsAtEnd{ false };
    const float m_EndStartPos{ 2500.f };
	const float m_EndLeftBoundary{ 2500.f };
};