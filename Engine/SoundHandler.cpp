#include "base.h"
#include "SoundHandler.h"

#include <iostream>

#include "SoundEffect.h"

SoundHandler::~SoundHandler()
{
	for (const std::pair<std::string, SoundEffect*> effect : m_SoundEffects)
	{
		delete effect.second;
	}
}

void SoundHandler::AddSoundEffect(const std::string& name, const std::string& file)
{
	m_SoundEffects[name] = new SoundEffect(file);
}

void SoundHandler::PlaySoundEffect(const std::string& name, int loops)
{
	if (!m_SoundEffects[name]->Play(loops))
	{
		std::cout << "Failed to play SFX " << name << std::endl;
	}
}

void SoundHandler::StopAll()
{
	for (auto& pair : m_SoundEffects)
	{
		pair.second->StopAll();
	}
}
