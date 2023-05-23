#pragma once

#include "Renderer.h"
#include <unordered_map>

class AnimatorState;
class AnimatorTransition;

class AnimatorRenderer final : public Renderer
{
public:
    explicit AnimatorRenderer(
        Entity* parent,
        Texture* texture,
        std::unordered_map<std::string, AnimatorState*> states,
        std::list<AnimatorTransition*> transitions,
        const std::string& entryState
    );

    AnimatorRenderer& operator=(const AnimatorRenderer& rhs) = delete;
    AnimatorRenderer& operator=(AnimatorRenderer&& rhs) = delete;
    AnimatorRenderer(const AnimatorRenderer& rhs) = delete;
    AnimatorRenderer(AnimatorRenderer&& rhs) = delete;

    ~AnimatorRenderer() override;

    void Initialize() override;

    int GetParameter(const std::string& paramName);
    void SetParameter(const std::string& paramName, int newValue);

    AnimatorState* GetCurrentState() const;
    std::unordered_map<std::string, AnimatorState*> GetStates() const;

    void SetState(const std::string& newState);

    void SetPaused(bool value);
    bool IsPaused() const;

    void Update(float deltaTime) override;
    void DrawSprite() const override;

    float GetCurrentStateTime() const;
    float GetCurrentLoopTime() const;
    int GetCurrentLoopFrame() const;

private:
    std::unordered_map<std::string, AnimatorState*> m_States;
    std::list<AnimatorTransition*> m_Transitions;
    std::unordered_map<std::string, int> m_Parameters;

    AnimatorState* m_CurrentState;

    bool m_IsPaused{ false };
    // The time the animator has been in the current state
    float m_CurrentStateTime{ 0.f };
    // Time until animation has to be looped
    float m_CurrentLoopTime{ 0.f };
    int m_CurrentLoopFrame{ 0 };
};