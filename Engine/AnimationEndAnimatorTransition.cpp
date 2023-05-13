#include "base.h"
#include "AnimationEndAnimatorTransition.h"

#include "Animation.h"
#include "AnimatorRenderer.h"
#include "AnimatorState.h"

AnimationEndAnimatorTransition::AnimationEndAnimatorTransition(const std::string& origin, const std::string& destination)
	: AnimatorTransition(origin, destination)
{
}

bool AnimationEndAnimatorTransition::ShouldTransition()
{
	// check if we've been in the state for at least the time of the animation
	const AnimatorState* transitionOriginState{ m_pAnimator->GetStates()[m_OriginState] };
	const float originAnimDuration{ transitionOriginState->GetAnimation()->GetTotalDuration() };
	return m_pAnimator->GetCurrentStateTime() >= originAnimDuration;
}
