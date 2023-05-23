#pragma once
#include <map>
#include "AnimatorTransition.h"

class ConditionalAnimatorTransition final : public AnimatorTransition
{
public:
    ConditionalAnimatorTransition(
        const std::string& origin,
        const std::string& destination,
        const std::map<std::string, int>& conditions
    );
    // Handy if there's only one transition condition
    ConditionalAnimatorTransition(
        const std::string& origin,
        const std::string& destination,
        const std::string& condition,
        int targetValue
    );

    bool ShouldTransition() override;

private:
    std::map<std::string, int> m_Conditions;
};

