#include "base.h"
#include "SoundHandler.h"

#include <iostream>

#include "SoundEffect.h"
#include "SoundStream.h"

SoundHandler::~SoundHandler()
{
	for (const std::pair<std::string, SoundEffect*> effect : m_SoundEffects)
	{
		delete effect.second;
	}
	for (const std::pair<std::string, SoundStream*> music : m_Music)
	{
		delete music.second;
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

void SoundHandler::AddMusic(const std::string& name, const std::string& file)
{
	m_Music[name] = new SoundStream(file);
}

void SoundHandler::PlayMusic(const std::string& name, bool repeat)
{
	if (!m_Music[name]->Play(repeat))
	{
		std::cout << "Failed to play Music " << name << std::endl;
	}
}

void SoundHandler::StopAll() const
{
	for (const auto& pair : m_SoundEffects)
	{
		pair.second->StopAll();
	}
	for (const auto& pair : m_Music)
	{
		pair.second->Stop();
	}
}
