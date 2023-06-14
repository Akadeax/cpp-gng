#include "pch.h"
#include "HudHandler.h"

#include <sstream>

#include "constants.h"
#include "LevelScene.h"
#include "Texture.h"
#include "TextureCache.h"

HudHandler::HudHandler(LevelScene* pLevelScene)
	: m_pLevelScene{ pLevelScene }
{
	m_pNumbersTexture = m_pLevelScene->GetTextureCache()->LoadTexture("numbers", "numbers.png");
}

void HudHandler::Draw() const
{
	glPushMatrix();
	glScalef(m_ScoreScale, m_ScoreScale, 1);
	glTranslatef(m_ScoreTopLeftOffset, (constants::SCREEN_HEIGHT - m_ScoreTopLeftOffset) / m_ScoreScale, 0);
	
	for(size_t i{ 0 }; i < m_DigitString.length(); ++i)
	{
		const char currentDigit{ m_DigitString[i] };
		const int currentDigitNum{ currentDigit - '0' };

		const float digitSize{ 7 };
		Rectf src{ digitSize * static_cast<float>(currentDigitNum), digitSize, digitSize, digitSize};

		if (i != 0)
		{
			glTranslatef(m_ScoreSpaceBetweenDigits, 0, 0);
		}
		m_pNumbersTexture->DrawCentered(src);
	}

	glPopMatrix();
}

void HudHandler::AddScore(int num)
{
	m_Score += num;

	// Update digits string each time score changes
	std::ostringstream os;
	os << m_Score;
	m_DigitString = os.str();
}
