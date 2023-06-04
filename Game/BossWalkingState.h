#pragma once
#include "AnimatorState.h"

class BossWalkingState final : public AnimatorState
{
public:
	explicit BossWalkingState(Animation* anim);

	void OnStateEnter(AnimatorRenderer* animator) override;
	void OnStateUpdate(AnimatorRenderer* animator, float deltaTime) override;
	void OnStateExit(AnimatorRenderer* animator) override;
};

