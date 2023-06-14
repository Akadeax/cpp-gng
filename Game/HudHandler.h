#pragma once
class Texture;
class LevelScene;

class HudHandler final
{
public:
	explicit HudHandler(LevelScene* pLevelScene);

	void Draw() const;

	void AddScore(int num);

private:
	LevelScene* m_pLevelScene;

	Texture* m_pNumbersTexture;

	int m_Score{ 100 };
	std::string m_DigitString{};

	const float m_ScoreTopLeftOffset{ 20.f };
	const float m_ScoreSpaceBetweenDigits{ 10.f };
	const float m_ScoreScale{ 3.f };
};
