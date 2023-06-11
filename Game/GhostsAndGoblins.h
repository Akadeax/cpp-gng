#pragma once
#include "Game.h"

class GhostsAndGoblins final : public Game
{
public:
	explicit GhostsAndGoblins(const Window& window);

	void InitializeGame() override;
	void UpdateGame(float deltaTime) override;
	void DrawGame() const override;
};

