#include "base.h"

#include <cassert>

#include "AnimatorRenderer.h"
#include "AnimatorTransition.h"
#include "AnimationFrame.h"
#include "Animation.h"
#include "Texture.h"
#include "AnimatorState.h"
#include "Entity.h"
#include "Transform.h"

AnimatorRenderer::AnimatorRenderer(
	Entity * parent,
	Texture * texture,
	std::unordered_map<std::string, AnimatorState*> states,
	std::list<AnimatorTransition*> transitions,
	const std::string& entryState
)   : Renderer(parent, texture)
	, m_States{std::move(states)}
	, m_Transitions{std::move(transitions)}
{
	m_CurrentState = m_States[entryState];

	for (AnimatorTransition* transition : m_Transitions)
	{
		transition->SetAnimatorRef(this);
	}
}

AnimatorRenderer::~AnimatorRenderer()
{
	for (const std::pair<std::string, AnimatorState*> state : m_States)
	{
		delete state.second;
	}
	for (const AnimatorTransition* transition : m_Transitions)
	{
		delete transition;
	}
}

void AnimatorRenderer::Initialize()
{
	m_pParentTransform = m_pParent->GetComponent<Transform>();
	assert(m_pParentTransform != nullptr && "Entity has renderer component but not transform component");

	for (const std::pair<std::string, AnimatorState*> state : m_States)
	{
		state.second->Initialize();
	}

	m_CurrentState->OnStateEnter(this);
}

int AnimatorRenderer::GetParameter(const std::string& paramName)
{
	return m_Parameters[paramName];
}

void AnimatorRenderer::SetParameter(const std::string& paramName, int newValue)
{
	m_Parameters[paramName] = newValue;
}

AnimatorState* AnimatorRenderer::GetCurrentState() const
{
	return m_CurrentState;
}

std::unordered_map<std::string, AnimatorState*> AnimatorRenderer::GetStates() const
{
	return m_States;
}

void AnimatorRenderer::SetState(const std::string& newState)
{
	if (m_States[newState] == m_CurrentState) return;

	m_CurrentLoopFrame = 0;
	m_CurrentLoopTime = 0;
	m_CurrentStateTime = 0;

	m_CurrentState->OnStateExit(this);
	m_CurrentState = m_States[newState];
	m_CurrentState->OnStateEnter(this);
}

void AnimatorRenderer::SetPaused(bool value)
{
	m_IsPaused = value;
}


bool AnimatorRenderer::IsPaused() const
{
	return m_IsPaused;
}

void AnimatorRenderer::Update(float deltaTime)
{
	if (IsPaused()) return;

	m_CurrentState->OnStateUpdate(this, deltaTime);

	m_CurrentStateTime += deltaTime;
	m_CurrentLoopTime += deltaTime;


	if (m_CurrentLoopTime >= m_CurrentState->GetAnimation()->GetTotalDuration())
	{
		m_CurrentLoopTime = 0;
	}

	// Determine current animation frame
	float currentIntervalEnd = 0;
	const std::vector<AnimationFrame*> frames = m_CurrentState->GetAnimation()->GetFrames();
	for (size_t i{ 0 }; i < frames.size(); i++)
	{
		currentIntervalEnd += frames[i]->GetDuration();
		if (m_CurrentLoopTime < currentIntervalEnd)
		{
			m_CurrentLoopFrame = static_cast<int>(i);
			break;
		}
	}

	// Check whether animator should transition states
	for (AnimatorTransition* transition : m_Transitions)
	{
		const bool inCorrectOriginState = m_CurrentState == m_States[transition->GetOriginState()];
		const bool shouldTransition = transition->ShouldTransition();
		if (inCorrectOriginState && shouldTransition)
		{
			SetState(transition->GetDestinationState());
			break;
		}
	}
}

void AnimatorRenderer::DrawSprite() const
{
	const AnimationFrame* currentFrame = m_CurrentState->GetAnimation()->GetFrame(m_CurrentLoopFrame);
	m_pTexture->DrawCentered(currentFrame->GetSpriteData());
}

float AnimatorRenderer::GetCurrentStateTime() const
{
	return m_CurrentStateTime;
}

float AnimatorRenderer::GetCurrentLoopTime() const
{
	return m_CurrentLoopTime;
}

int AnimatorRenderer::GetCurrentLoopFrame() const
{
	return m_CurrentLoopFrame;
}