#pragma once
#include <unordered_map>

class SoundStream;
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

	void AddMusic(const std::string& name, const std::string& file);
	void PlayMusic(const std::string& name, bool repeat = true);

	void StopAll() const;

private:
	std::unordered_map<std::string, SoundEffect*> m_SoundEffects{};
	std::unordered_map<std::string, SoundStream*> m_Music{};
};

