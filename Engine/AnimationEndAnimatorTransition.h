#pragma once
#include "AnimatorTransition.h"

class AnimationEndAnimatorTransition final : public AnimatorTransition
{
public:
    explicit AnimationEndAnimatorTransition(const std::string& origin, const std::string& destination);

    bool ShouldTransition() override;
};

