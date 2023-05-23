#include "base.h"

#include "ConditionalAnimatorTransition.h"
#include "AnimatorRenderer.h"

ConditionalAnimatorTransition::ConditionalAnimatorTransition(const std::string& origin, const std::string& destination, const std::map<std::string, int>& conditions)
	: AnimatorTransition(origin, destination), m_Conditions{ conditions }
{
}

ConditionalAnimatorTransition::ConditionalAnimatorTransition(const std::string& origin, const std::string& destination,
	const std::string& condition, int targetValue)
		: ConditionalAnimatorTransition(origin, destination, std::map<std::string, int>{ { condition, targetValue} })
{
}

bool ConditionalAnimatorTransition::ShouldTransition()
{
	// Check if all required conditions match
	for (const std::pair<std::string, int> pair : m_Conditions)
	{
		if (m_pAnimator->GetParameter(pair.first) == pair.second)
		{
			continue;
		}

		return false;
	}

	return true;
}
