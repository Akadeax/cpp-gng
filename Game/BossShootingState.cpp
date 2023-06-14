#include "pch.h"
#include "BossShootingState.h"

#include "AnimatorRenderer.h"
#include "LevelScene.h"
#include "Entity.h"
#include "PhysicsBody.h"
#include "ProjectilePool.h"
#include "SoundHandler.h"
#include "Transform.h"

BossShootingState::BossShootingState(Animation* anim)
	: AnimatorState(anim)
{
}

void BossShootingState::OnStateEnter(AnimatorRenderer* animator)
{
	const Transform* pTransform{ animator->GetParent()->GetComponent<Transform>() };
	const LevelScene* pScene{ dynamic_cast<LevelScene*>(animator->GetParent()->GetScene()) };

	const float shootHeightOffset{ m_ShootHigh ? m_ShootHighOffset : m_ShootLowOffset };
	m_ShootHigh = !m_ShootHigh;

	const Projectile::FireData data
	{
		Projectile::Type::enemy,
		Vector2f(pTransform->GetPosition().x, pTransform->GetPosition().y + shootHeightOffset),
		Vector2f(-m_ProjectileSpeed, 0),
		3,
		5, 5
	};

	pScene->GetProjectilePool()->FireProjectile(data);
	animator->GetSoundHandler()->PlaySoundEffect("bossAttack");
}

void BossShootingState::OnStateUpdate(AnimatorRenderer* animator, float deltaTime)
{
	animator->GetParent()->GetComponent<PhysicsBody>()->SetXVelocity(0);
}
