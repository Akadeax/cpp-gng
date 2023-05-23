#pragma once
#include <vector>
#include "Vector2f.h"

namespace collisions
{
	struct CollisionHitInfo
	{
		bool hit = false;
		float depth = 0.f;
		Vector2f normal = Vector2f();

		explicit CollisionHitInfo(bool hit, float depth, Vector2f normal)
			: hit{ hit }, depth{ depth }, normal{ normal } {}
	};

	// Polygon Intersection using SAT (Seperating Axis Theorem)
	CollisionHitInfo IntersectPolygons(const std::vector<Vector2f>& verticesA, const std::vector<Vector2f>& verticesB);
	Vector2f FindArithmeticMean(const std::vector<Vector2f>& vertices);
	void ProjectVertices(const std::vector<Vector2f>& vertices, const Vector2f& axis, float& min, float& max);

	bool IntersectLinePolygon(const Vector2f& p1, const Vector2f& p2, const std::vector<Vector2f>& vertices);
	bool IntersectLines(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4);
	bool IntersectPointPolygon(const Vector2f& p1, const std::vector<Vector2f>& vertices);
}