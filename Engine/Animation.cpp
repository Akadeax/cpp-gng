#include "base.h"

#include "Animation.h"
#include "AnimationFrame.h"

Animation::Animation(const std::vector<AnimationFrame*>& frames)
	: m_Frames{ frames }
{
    for (const AnimationFrame* frame : frames)
    {
        m_TotalDuration += frame->GetDuration();
    }
}

Animation::~Animation()
{
    for (const AnimationFrame* frame : m_Frames)
    {
        delete frame;
        frame = nullptr;
    }
}

const AnimationFrame* Animation::GetFrame(int frameNr) const
{
    return m_Frames[frameNr];
}

const std::vector<AnimationFrame*>& Animation::GetFrames() const
{
    return m_Frames;
}

float Animation::GetTotalDuration() const
{
    return m_TotalDuration;
}
