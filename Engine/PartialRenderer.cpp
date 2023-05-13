#include "base.h"
#include "PartialRenderer.h"

#include "Texture.h"

PartialRenderer::PartialRenderer(Entity* pParent, Texture* pTexture, int spriteAmount, int startIndex, bool horizontal)
	: Renderer(pParent, pTexture)
	, m_Horizontal{ horizontal }
	, m_SpriteAmount{ spriteAmount }
	, m_Index{ startIndex }
{
}

void PartialRenderer::DrawSprite() const
{
	const float mainAxisTextureSize{ m_Horizontal ? m_pTexture->GetWidth() : m_pTexture->GetHeight() };
	const float mainAxisSpriteSize{ mainAxisTextureSize / static_cast<float>(m_SpriteAmount) };
	const float crossAxisSize{ m_Horizontal ? m_pTexture->GetHeight() : m_pTexture->GetWidth() };

	Rectf src;
	if (m_Horizontal) src = Rectf(static_cast<float>(m_Index) * mainAxisSpriteSize, crossAxisSize, mainAxisSpriteSize, crossAxisSize);
	else              src = Rectf(0, (static_cast<float>(m_Index) + 1) * mainAxisSpriteSize, crossAxisSize, mainAxisSpriteSize);


	m_pTexture->DrawCentered(src);
}

void PartialRenderer::SetIndex(int index)
{
	m_Index = index;
}
