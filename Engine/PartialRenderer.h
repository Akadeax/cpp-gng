#pragma once
#include "Renderer.h"

class PartialRenderer final : public Renderer
{
public:
	PartialRenderer(Entity* pParent, Texture* pTexture, int spriteAmount, int startIndex, bool horizontal = true);
	PartialRenderer& operator=(const PartialRenderer& rhs) = delete;
	PartialRenderer& operator=(PartialRenderer&& rhs) = delete;
	PartialRenderer(const PartialRenderer& rhs) = delete;
	PartialRenderer(PartialRenderer&& rhs) = delete;
	~PartialRenderer() override = default;

	void DrawSprite() const override;

	void SetIndex(int index);

private:
	bool m_Horizontal;
	const int m_SpriteAmount;
	int m_Index;
};
