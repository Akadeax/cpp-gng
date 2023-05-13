#pragma once
#include "Collider.h"

class PartialRenderer;

class Projectile final : public Collider
{
public:
	enum class Type
	{
		player, enemy
	};
	struct FireData
	{
		Type type;
		Vector2f start;
		Vector2f velocity;
		int spriteIndex;
		float width;
		float height;
	};
	
	explicit Projectile(Entity* pParent);
	void Initialize() override;

	void Update(float deltaTime) override;

	void SetFireData(const FireData& data);

	void OnCollisionEnter(Collider* other, float deltaTime) override;

private:
	PartialRenderer* m_pRenderer{ nullptr };

	Type m_Type;
	Vector2f m_Velocity{};

	const float m_MaxLifetime{ 10.f };
	float m_Lifetime{};
};
