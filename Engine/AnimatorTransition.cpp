#include "base.h"

#include "AnimatorTransition.h"

AnimatorTransition::AnimatorTransition(const std::string& origin, const std::string& destination)
	: m_OriginState{ origin }, m_DestinationState{ destination }
{
}

AnimatorTransition::~AnimatorTransition()
{
}

const std::string& AnimatorTransition::GetOriginState() const
{
	return m_OriginState;
}

const std::string& AnimatorTransition::GetDestinationState() const
{
	return m_DestinationState;
}

void AnimatorTransition::SetAnimatorRef(AnimatorRenderer* animator)
{
	m_pAnimator = animator;
}