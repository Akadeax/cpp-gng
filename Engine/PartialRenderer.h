#pragma once
#include "Renderer.h"

class PartialRenderer final : public Renderer
{
public:
	PartialRenderer(Entity* pParent, Texture* pTexture, int spriteAmount, int startIndex, bool horizontal = true);
	void DrawSprite() const override;

	void SetIndex(int index);

private:
	bool m_Horizontal;
	const int m_SpriteAmount;
	int m_Index;
};
