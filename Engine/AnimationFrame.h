#pragma once
#include "structs.h"

class AnimationFrame final
{
public:
	explicit AnimationFrame(float duration, const Rectf& spriteData);

	float GetDuration() const;
	Rectf GetSpriteData() const;

private:
	float m_Duration;
	Rectf m_SpriteData;
};