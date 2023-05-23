#pragma once
#include "Component.h"

class Texture;
class Transform;

class Renderer : public Component
{
public:
	explicit Renderer(Entity* pParent, Texture* pTexture);
	Renderer& operator=(const Renderer& rhs) = delete;
	Renderer& operator=(Renderer&& rhs) = delete;
	Renderer(const Renderer& rhs) = delete;
	Renderer(Renderer&& rhs) = delete;
	~Renderer() override = default;

	void Initialize() override;
	void Draw() const override;

	virtual void DrawSprite() const;

	bool GetFlipX() const;
	void SetFlipX(bool newState);
	bool GetFlipY() const;
	void SetFlipY(bool newState);

	void SetTexture(Texture* pTexture);

protected:
	Texture* m_pTexture;
	Transform* m_pParentTransform{ nullptr };

	bool m_FlipX = false;
	bool m_FlipY = false;
};