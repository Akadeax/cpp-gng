#pragma once
#include "structs.h"

class AnimationFrame final
{
public:
	explicit AnimationFrame(float duration, const Rectf& spriteData);
	AnimationFrame& operator=(const AnimationFrame& rhs) = delete;
	AnimationFrame& operator=(AnimationFrame&& rhs) = delete;
	AnimationFrame(const AnimationFrame& rhs) = delete;
	AnimationFrame(AnimationFrame&& rhs) = delete;
	~AnimationFrame() = default;

	float GetDuration() const;
	Rectf GetSpriteData() const;

private:
	float m_Duration;
	Rectf m_SpriteData;
};