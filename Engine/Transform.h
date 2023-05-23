#pragma once
#include "Component.h"
#include "Vector2f.h"

class Transform final : public Component
{
public:
	explicit Transform(Entity* parent);
	explicit Transform(Entity* parent, const Vector2f& startingPos);
	Transform& operator=(const Transform& rhs) = delete;
	Transform& operator=(Transform&& rhs) = delete;
	Transform(const Transform& rhs) = delete;
	Transform(Transform&& rhs) = delete;
	~Transform() override = default;

	const Vector2f& GetPosition() const;

	void SetPosition(const Vector2f& newPos);
	void MovePosition(const Vector2f& delta);

	void SetXPosition(float newX);
	void SetYPosition(float newY);

	float GetRotation() const;
	void SetRotation(float newRotation);
	void Rotate(float delta);

	float GetScale() const;
	void SetScale(float newScale);


private:
	Vector2f m_Position;
	float m_RotationDeg;
	float m_Scale;
};
