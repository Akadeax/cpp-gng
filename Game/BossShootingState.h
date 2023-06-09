#pragma once
#include "AnimatorState.h"

class BossShootingState final : public AnimatorState
{
public:
	explicit BossShootingState(Animation* anim);

	void OnStateEnter(AnimatorRenderer* animator) override;
	void OnStateUpdate(AnimatorRenderer* animator, float deltaTime) override;

private:
	const float m_ProjectileSpeed{ 90.f };

	bool m_ShootHigh{ false };
	const float m_ShootHighOffset{ 1.f };
	const float m_ShootLowOffset{ -10.f };
};

