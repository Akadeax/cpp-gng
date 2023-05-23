#pragma once
#include "Component.h"
#include <vector>
#include "Vector2f.h"

class Transform;

class Collider : public Component
{
public:
	explicit Collider(Entity* pParent, const std::vector<Vector2f>& vertices, bool isTrigger = false);
	Collider& operator=(const Collider& rhs) = delete;
	Collider& operator=(Collider&& rhs) = delete;
	Collider(const Collider& rhs) = delete;
	Collider(Collider&& rhs) = delete;
	~Collider() override;

	static Collider* FromBottomLeft(Entity* pParent, const Vector2f& bottomLeft, const Vector2f& size, bool isTrigger = false);
	static Collider* FromCenter(Entity* pParent, const Vector2f& center, const Vector2f& size, bool isTrigger = false);

	void Initialize() override;

	virtual void OnCollisionUpdate(Collider* other, float deltaTime);
	virtual void OnCollisionEnter(Collider* other, float deltaTime);
	virtual void OnCollisionExit(Collider* other, float deltaTime);

	void DrawDebugCollider() const;

	void RecalculateTransformedVertices();

	bool IsTrigger() const;
	void SetTrigger(bool newVal);

	bool CompareTag(const std::string& tag) const;

	std::vector<Vector2f>& GetBaseVertices();
	void SetBaseVertices(const std::vector<Vector2f>& newVertices);
	const std::vector<Vector2f>& GetTransformedVertices();

	Transform* GetTransform() const;

	void SetEnabled(bool value);
	bool IsEnabled() const;

	float GetHeight() const;
protected:
	Transform* m_pTransform{ nullptr };

	// Whether the collider is pass-through
	bool m_IsTrigger{ false };

	bool m_IsEnabled{ true };

	// Non-transformed vertices in relation to the transforms position
	std::vector<Vector2f> m_BaseVertices;
	// Computed (cached) transformed vertices
	std::vector<Vector2f> m_TransformedVertices;
};