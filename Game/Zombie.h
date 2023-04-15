#pragma once
#include "Component.h"

class Collider;
class AnimatorRenderer;
class PhysicsBody;
class Transform;
class LevelScene;
class Texture;

class Zombie final : public Component
{
public:
	explicit Zombie(Entity* pParent, LevelScene* pLevelScene);
	void Initialize() override;
	void Update(float deltaTime) override;

	void Damage();

	void Reset();

private:
	LevelScene* m_pLevelScene;

	Transform* m_pTransform{ nullptr };
	Collider* m_pCollider{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };
	AnimatorRenderer* m_pAnimator{ nullptr };

	Transform* m_pPlayer{ nullptr };

	const float m_WalkSpeed{ 30.f };

	bool m_IsDead{ false };
	const float m_DeathTime{ 0.3f };
	float m_CurrentDeathTime{};
};
