#include "BoundingBox.h"

namespace PhysicsEngine
{
	void InitializeAABB(AABB& a, vec3 min, vec3 max)
	{
		a.min = min;
		a.max = max;
	}

	void ComputeAABB(AABB& aabb, const std::vector<ShapesEngine::Vertex>& vertices)
	{
		vec3 minP(vertices.at(0).position);
		vec3 maxP(vertices.at(0).position);

		//find the min and max x,y and z values.
		for (const auto& i : vertices)
		{
			if (i.position.x < minP.x)
				minP.x = i.position.x;

			if (i.position.x > maxP.x)
				maxP.x = i.position.x;

			if (i.position.y < minP.y)
				minP.y = i.position.y;

			if (i.position.y > maxP.y)
				maxP.y = i.position.y;

			if (i.position.z < minP.z)
				minP.z = i.position.z;

			if (i.position.z > maxP.z)
				maxP.z = i.position.z;
		}

		aabb.min = minP;
		aabb.max = maxP;
	}

	void TransformAABB(AABB& worldAABB, const AABB& localAABB, const mat4& model)
	{
		worldAABB.min = vec3{};
		worldAABB.max = vec3{};

		float a{ 0.0f };
		float b{ 0.0f };

		//xmin, xmax
		a = localAABB.min.x * model(0, 0);
		b = localAABB.max.x * model(0, 0);
		worldAABB.min.x += a < b ? a : b;
		worldAABB.max.x += a < b ? b : a;

		a = localAABB.min.y * model(1, 0);
		b = localAABB.max.y * model(1, 0);
		worldAABB.min.x += a < b ? a : b;
		worldAABB.max.x += a < b ? b : a;

		a = localAABB.min.z * model(2, 0);
		b = localAABB.max.z * model(2, 0);
		worldAABB.min.x += a < b ? a : b;
		worldAABB.max.x += a < b ? b : a;

		worldAABB.min.x += model(3, 0);
		worldAABB.max.x += model(3, 0);

		//ymin, ymax
		a = localAABB.min.x * model(0, 1);
		b = localAABB.max.x * model(0, 1);
		worldAABB.min.y += a < b ? a : b;
		worldAABB.max.y += a < b ? b : a;

		a = localAABB.min.y * model(1, 1);
		b = localAABB.max.y * model(1, 1);
		worldAABB.min.y += a < b ? a : b;
		worldAABB.max.y += a < b ? b : a;

		a = localAABB.min.z * model(2, 1);
		b = localAABB.max.z * model(2, 1);
		worldAABB.min.y += a < b ? a : b;
		worldAABB.max.y += a < b ? b : a;

		worldAABB.min.y += model(3, 1);
		worldAABB.max.y += model(3, 1);

		//zmin, zmax
		a = localAABB.min.x * model(0, 2);
		b = localAABB.max.x * model(0, 2);
		worldAABB.min.z += a < b ? a : b;
		worldAABB.max.z += a < b ? b : a;

		a = localAABB.min.y * model(1, 2);
		b = localAABB.max.y * model(1, 2);
		worldAABB.min.z += a < b ? a : b;
		worldAABB.max.z += a < b ? b : a;

		a = localAABB.min.z * model(2, 2);
		b = localAABB.max.z * model(2, 2);
		worldAABB.min.z += a < b ? a : b;
		worldAABB.max.z += a < b ? b : a;

		worldAABB.min.z += model(3, 2);
		worldAABB.max.z += model(3, 2);
	}

	bool TestIntersection(const AABB& a, const AABB& b)
	{
		//Two AABBs intersect if and only if they are intersecting on all three axes.
		//Return false if they are not intersecting on one axis.

		//x-axis test
		if (a.max.x < b.min.x || a.min.x > b.max.x)
			return false;

		//y-axis test
		if (a.max.y < b.min.y || a.min.y > b.max.y)
			return false;

		//z-axis test
		if (a.max.z < b.min.z || a.min.z > b.max.z)
			return false;

		//They intersect on all axes.
		return true;
	}



	BoundingBox::BoundingBox()
	{}

	BoundingBox::BoundingBox(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color)
	{
		InitializeBoundingBox(vertices, color);
	}

	void BoundingBox::InitializeBoundingBox(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color)
	{
		ComputeAABB(mLocalAABB, vertices);

		mRenderObject.color = color;
	}

	void BoundingBox::UpdateModelMatrix()
	{
		vec3 scale{ mWorldAABB.max - mWorldAABB.min };
		vec3 translate{ (mWorldAABB.min + mWorldAABB.max) * 0.5f };
		mRenderObject.modelMatrix = MathEngine::Scale4x4(scale) * MathEngine::Translate(translate);
	}

	void BoundingBox::TransformBoundingVolume(const mat4& model)
	{
		TransformAABB(mWorldAABB, mLocalAABB, model);
	}
}