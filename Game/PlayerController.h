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

	void Damage(Vector2f from);
    // Callback from PlayerCollider
    void Event(std::string name);
    void Event(std::string name, Vector2f move, Vector2f accel);

    // move this to a ground collider event?
private:
	Transform* m_pTransform{ nullptr };
	PlayerCollider* m_pCollider{ nullptr };
	PhysicsBody* m_pPhysicsBody{ nullptr };
	AnimatorRenderer* m_pAnimator{ nullptr };

    float m_inhibitTimer{ 0.f };
	bool m_HasArmor{ true };
	float m_HurtTimer{ 0.f };

	bool m_IsGrounded{ false };
	bool m_IsCrouched{ false };
	int m_LookDir{ 1 };

	void UpdateGroundMovement();
	void UpdateLadderMovement();
	void CheckCollisions(float deltaTime);
	void UpdateJumping();
	void UpdateShooting(float deltaTime);
	void UpdateHurt();

	const float m_GravityScale{ 175.f };

	const float m_ColliderWidth{ 22.f };
	const float m_ColliderHeight{ 25.f };

	const float m_MovementSpeed{ 50.f };
	const float m_JumpForce{ 100.f };

	const float m_CrouchedHitboxHeightMultiplier{ 0.5f };

	const float m_ShootCooldown{ 0.2f };
	float m_CurrentShootCooldown{};

	const float m_ShootTime{ 0.1f };
	float m_CurrentShootTime{};

	bool m_IsShooting{ false };
	const float m_ShootingSpeed{ 200.f };

	const float m_ClimbSpeed{ 50.f };
	bool m_IsClimbing{ false };

	const float m_DamagedInactiveTime{ 0.5f };
	const float m_DamagedHorizontalVelocity{ 50.f };
	const float m_DamagedVerticalVelocity{ 50.f };
};
