#pragma once
#include "Component.h"

struct Vector2f;
class PlayerCollider;
class AnimatorRenderer;
class Transform;
class PhysicsBody;

class PlayerController final : public Component
{
public:
	explicit PlayerController(Entity* parent);

	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() const override;

	void Damage(const Vector2f& from);

private:
	Transform* m_pTransform{ nullptr };
	PlayerCollider* m_pCollider{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };
	AnimatorRenderer* m_pAnimator{ nullptr };

	bool m_HasArmor{ true };
	float m_HurtTimer{ 0.f };

	bool m_IsGrounded{ false };
	bool m_IsCrouched{ false };
	int m_LookDir{ 1 };

	void UpdateGroundMovement();
	void UpdateLadderMovement();
	void CheckGrounded(float deltaTime);
	void UpdateJumping() const;
	void UpdateShooting(float deltaTime);
	void UpdateHurt(float deltaTime);

	const float m_GravityScale{ 400.f };

	const float m_ColliderWidth{ 22.f };
	const float m_ColliderHeight{ 25.f };

	const float m_MovementSpeed{ 60.f };
	const float m_JumpForce{ 130.f };

	const float m_CrouchedHitboxHeightMultiplier{ 0.5f };

	const float m_ShootCooldown{ 0.3f };
	float m_CurrentShootCooldown{};

	const float m_ShootTime{ 0.1f };
	float m_CurrentShootTime{};

	bool m_IsShooting{ false };
	const float m_ShootingSpeed{ 150.f };
	const float m_ShootProjectileOffset{ 2.f };
	const float m_ShootProjectileOffsetCrouched{ -3.f };

	const float m_ClimbSpeed{ 50.f };
	bool m_IsClimbing{ false };

	const float m_DamagedInactiveTime{ 0.5f };
	const float m_DamagedHorizontalVelocity{ 80.f };
	const float m_DamagedVerticalVelocity{ 80.f };

	bool m_IsDead{ false };

	float m_DeathResetTime{ 3.f };
};
