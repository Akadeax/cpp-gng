#pragma once
#include "AnimatorState.h"

class PhysicsBody;



class BossWalkingState final : public AnimatorState
{
public:
	explicit BossWalkingState(Animation* anim);

	void OnStateEnter(AnimatorRenderer* animator) override;
	void OnStateUpdate(AnimatorRenderer* animator, float deltaTime) override;
	void OnStateExit(AnimatorRenderer* animator) override;

private:
	enum MoveDirection
	{
		left, right
	};

	PhysicsBody* m_pPhysicsBody{ nullptr };

	const float m_MoveSpeed{ 40.f };

	const float m_MoveTime{ 1.0f };
	float m_CurrentMoveTime{ 0.f };
	MoveDirection m_Direction{ left };
};

