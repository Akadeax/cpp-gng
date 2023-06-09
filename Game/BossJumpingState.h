#pragma once
#include "AnimatorState.h"

class PhysicsBody;
class Transform;

class BossJumpingState final : public AnimatorState
{
public:
	explicit BossJumpingState(Animation* anim);

	void OnStateEnter(AnimatorRenderer* animator) override;
	void OnStateUpdate(AnimatorRenderer* animator, float deltaTime) override;
	void OnStateExit(AnimatorRenderer* animator) override;

private:
	Transform* m_pTransform{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };

	const float m_JumpStrength{ 100.f };
	const float m_GravityStrength{ 150.f };
	float m_StartingHeight{};
};

