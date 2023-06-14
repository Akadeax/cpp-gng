#include "pch.h"
#include "GhostsAndGoblins.h"

#include "InputHandler.h"
#include "LevelScene.h"
#include "SoundHandler.h"

GhostsAndGoblins::GhostsAndGoblins(const Window& window)
	: Game(window, new LevelScene())
{
}

void GhostsAndGoblins::InitializeGame()
{
	m_pInputHandler->AddKey("information", SDL_SCANCODE_I);

	m_pInputHandler->AddKey("left", SDL_SCANCODE_LEFT);
	m_pInputHandler->AddKey("right", SDL_SCANCODE_RIGHT);
	m_pInputHandler->AddKey("up", SDL_SCANCODE_UP);
	m_pInputHandler->AddKey("down", SDL_SCANCODE_DOWN);
	m_pInputHandler->AddKey("jump", SDL_SCANCODE_J);
	m_pInputHandler->AddKey("fire", SDL_SCANCODE_K);
	m_pInputHandler->AddKey("pause", SDL_SCANCODE_ESCAPE);

	m_pSoundHandler->AddMusic("stage01", "SFX/stage01.wav");

	m_pSoundHandler->AddSoundEffect("arthurDeath", "SFX/arthur_death.wav");
	m_pSoundHandler->AddSoundEffect("arthurHit", "SFX/arthur_hit.wav");
	m_pSoundHandler->AddSoundEffect("arthurJump", "SFX/arthur_jump.wav");
	m_pSoundHandler->AddSoundEffect("arthurThrow", "SFX/arthur_throw.wav");

	m_pSoundHandler->AddSoundEffect("bossHit", "SFX/boss_hit.wav");
	m_pSoundHandler->AddSoundEffect("bossDeath", "SFX/boss_death.wav");
	m_pSoundHandler->AddSoundEffect("bossAttack", "SFX/boss_attack.wav");
	m_pSoundHandler->AddSoundEffect("bossJump", "SFX/boss_jump.wav");

	m_pSoundHandler->AddSoundEffect("enemyHit", "SFX/enemy_hit.wav");
	m_pSoundHandler->AddSoundEffect("enemyDeath", "SFX/enemy_death.wav");

	m_pSoundHandler->AddSoundEffect("zombieSpawn", "SFX/zombie_spawn.wav");
}

void GhostsAndGoblins::UpdateGame(float deltaTime)
{
}

void GhostsAndGoblins::DrawGame() const
{
}
