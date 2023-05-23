#pragma once
#include "Collider.h"
#include "Component.h"

class Transform;

class MovingPlatform final : public Collider
{
public:
	explicit MovingPlatform(Entity* pParent, const std::vector<Vector2f>& vertices, float xStart, float xEnd);

	void Initialize() override;

	void Update(float deltaTime) override;

	void OnCollisionEnter(Collider* other, float deltaTime) override;
	void OnCollisionExit(Collider* other, float deltaTime) override;

private:
	float m_XStart;
	float m_XEnd;

	Transform* m_pCollidingPlayer{};

	float m_Time{ 0.f };
	float m_YPos{};
};

