#include "pch.h"
#include "ProjectilePool.h"

#include "Entity.h"
#include "EntityKeeper.h"
#include "LevelScene.h"
#include "PartialRenderer.h"
#include "Projectile.h"
#include "TextureCache.h"
#include "Transform.h"

ProjectilePool::ProjectilePool(const LevelScene* pLevelScene)
{
	EntityKeeper* pEntityKeeper{ pLevelScene->GetEntityKeeper() };

	const int projectilePoolAmount{ 10 };

	Texture* pProjectilesTexture{ pLevelScene->GetTextureCache()->LoadTexture("projectiles", "Resources/projectiles.png")};
	m_Projectiles = std::vector<Projectile*>(projectilePoolAmount);

	for (size_t i{ 0 }; i < projectilePoolAmount; ++i)
	{
		Entity* pEntity{ pEntityKeeper->CreateEntity() };

		pEntity->AddComponent(new Transform(pEntity));
		Projectile* pProjectile{ new Projectile(pEntity) };
		pEntity->AddComponent(pProjectile);

		pEntity->AddComponent(new PartialRenderer(pEntity, pProjectilesTexture, 3, 1, false));

		pEntity->SetActive(false);
		pEntity->Initialize();

		m_Projectiles[i] = pProjectile;
	}
}

void ProjectilePool::FireProjectile(const Projectile::FireData& data) const
{
	for(size_t i{ 0 }; i < m_Projectiles.size(); ++i)
	{
		if (m_Projectiles[i]->GetParent()->IsActive())
		{
			continue;
		}

		m_Projectiles[i]->SetFireData(data);
		m_Projectiles[i]->GetParent()->SetActive(true);
		return;
	}
}
