#pragma once
#include <unordered_map>

class SoundEffect;

class SoundHandler final
{
public:
	SoundHandler() = default;
	SoundHandler(const SoundHandler&) = delete;
	SoundHandler(SoundHandler&&) = delete;
	SoundHandler& operator=(const SoundHandler&) = delete;
	SoundHandler& operator=(SoundHandler&&) = delete;
	~SoundHandler();

	void AddSoundEffect(const std::string& name, const std::string& file);
	void PlaySoundEffect(const std::string& name, int loops = 0);

	void StopAll();

private:
	std::unordered_map<std::string, SoundEffect*> m_SoundEffects{};
};

