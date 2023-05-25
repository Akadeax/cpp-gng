#include "base.h"
#include "Renderer.h"

#include <cassert>

#include "Entity.h"
#include "Texture.h"
#include "Transform.h"
#include "utils.h"

Renderer::Renderer(Entity* pParent, Texture* pTexture, bool centered)
	: Component(pParent)
	, m_pTexture{ pTexture }
	, m_Centered{ centered }
{
}

void Renderer::Initialize()
{
	m_pParentTransform = m_pParent->GetComponent<Transform>();
	assert(m_pParentTransform != nullptr && "Entity has renderer component but not transform component");
}

void Renderer::Draw() const
{

	glTranslatef(m_pParentTransform->GetPosition().x, m_pParentTransform->GetPosition().y, 0);

	const float renderScale = m_pParentTransform->GetScale();
	glScalef(
		m_FlipX ? -renderScale : renderScale,
		m_FlipY ? -renderScale : renderScale,
		0
	);

	DrawSprite();

#ifdef _DEBUG
	utils::SetColor(Color4f(0, 1, 1, 1));
	utils::FillEllipse(0, 0, 3, 3);
#endif
}

void Renderer::DrawSprite() const
{
	if (m_Centered)
	{
		m_pTexture->DrawCentered();
	}
	else
	{
		m_pTexture->Draw();
	}
}

bool Renderer::GetFlipX() const
{
	return m_FlipX;
}

void Renderer::SetFlipX(bool newState)
{
	m_FlipX = newState;
}

bool Renderer::GetFlipY() const
{
	return m_FlipY;
}

void Renderer::SetFlipY(bool newState)
{
	m_FlipY = newState;
}

void Renderer::SetTexture(Texture* pTexture)
{
	m_pTexture = pTexture;
}
