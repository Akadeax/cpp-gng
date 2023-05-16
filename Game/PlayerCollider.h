#pragma once
#include "Collider.h"

class LadderCollider;
class AnimatorRenderer;
class PlayerController;

class PlayerCollider final : public Collider
{
public:
	explicit PlayerCollider(Entity* pParent);

	void Initialize() override;
	void OnCollisionEnter(Collider* other, float deltaTime) override;
	void OnCollisionExit(Collider* other, float deltaTime) override;

	bool IsTouchingLadder() const;
	LadderCollider* GetTouchedLadder() const;

	void SetCanInteract(bool newVal);

private:
	PlayerController* m_pPlayerController{ nullptr };

	LadderCollider* m_pTouchedLadder{ nullptr };

	bool m_CanInteract{ true };
};
