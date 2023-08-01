#include "BoundingSphere.h"

namespace PhysicsEngine
{
	void InitalizeSphere(Sphere& sphere, const vec3& center, float radius)
	{
		sphere.center = center;
		sphere.radius = (radius <= 0.0) ? 1.0f : radius;
	}

	void ComputeSphere(Sphere& sphere, const std::vector<ShapesEngine::Vertex>& vertices)
	{
		vec3 minP(vertices.at(0).position);
		vec3 maxP(vertices.at(0).position);

		//find the min and max x, y and z values.
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

		//Compute the distances between each interval
		float distanceX{ maxP.x - minP.x };

		float distanceY{ maxP.y - minP.y };

		float distanceZ{ maxP.z - minP.z };

		//The max distance is the diameter of the sphere
		float diameter{ distanceX };

		if (distanceY > diameter)
			diameter = distanceY;

		if (distanceZ > diameter)
			diameter = distanceZ;

		//Compute the properties for the initial sphere.
		sphere.center = (maxP + minP) * 0.5f; //midpoint between the min and max points
		sphere.radius = diameter / 2.0f; //radius = diameter / 2

		//Check if the sphere bounds all points of the object. If a point is outside the sphere create a new sphere that bounds the old sphere and the point.
		for (const auto& i : vertices)
		{
			//vector from the center of the sphere to the point
			vec3 distanceVector(i.position - sphere.center);

			//sqaured distance of the vector from the center of the sphere to the point
			float distance{ MathEngine::DotProduct(distanceVector, distanceVector) };

			//The point is outside the sphere if the squared distance between spheres center and the point is greater than the squared radius of the sphere.
			if (distance > sphere.radius * sphere.radius)
			{
				//Update sphere to bound the the old sphere and point.

				//Compute non-sqaured distance
				distance = sqrt(distance);

				//direction the spheres center is going to move in.
				vec3 direction(MathEngine::Normalize(distanceVector));

				//Adding the old radius to the distance from the center of the sphere to the point gives you the new diameter of the sphere.
				//Divide by 2 to get the new radius.
				float newRadius{ (sphere.radius + distance) * 0.5f };

				//The distance on how much to move the center of the sphere to its new center is new radius - old radius
				float k{ newRadius - sphere.radius };

				sphere.radius = newRadius;
				sphere.center += direction * k;
			}
		}
	}

	void TransformSphere(Sphere& worldSphere, const Sphere& localSphere, const mat4& model)
	{
		//The last row of the model matrix has the translation
		worldSphere.center = localSphere.center + vec3{ model(3, 0), model(3, 1), model(3, 2) };

		//The first 3 rows and columns of a model matrix is rotation and scale.
		//The rotation matrix is orthonormal which means its rows have a magnitdue of 1.
		//To retrieve the scale factors, get the magnitdue of the rows.
		float scaleX{ MathEngine::Length(vec3{ model(0, 0), model(0, 1), model(0, 2) }) };
		float scaleY{ MathEngine::Length(vec3{ model(1, 0), model(1, 1), model(1, 2) }) };
		float scaleZ{ MathEngine::Length(vec3{ model(2, 0), model(2, 1), model(2, 2) }) };

		float maxScale{ scaleX };
		if (scaleY > maxScale)
			maxScale = scaleY;
		if (scaleZ > maxScale)
			maxScale = scaleZ;

		worldSphere.radius = localSphere.radius * maxScale;
	}

	bool TestIntersection(const Sphere& a, const Sphere& b)
	{
		vec3 distanceVector{ a.center - b.center };

		//compute the square of the distance between the centers
		float distance{ MathEngine::DotProduct(distanceVector, distanceVector) };

		float radiusSum{ a.radius + b.radius };

		//Two spheres intersect if the squared distance between their centers <= the squared sum of their radii
		return distance <= radiusSum * radiusSum;
	}



	BoundingSphere::BoundingSphere()
	{}

	BoundingSphere::BoundingSphere(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color)
	{
		InitializeBoundingSphere(vertices, color);
	}

	void BoundingSphere::InitializeBoundingSphere(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color)
	{
		ComputeSphere(mLocalBoundingSphere, vertices);

		mRenderObject.color = color;
	}

	void BoundingSphere::UpdateModelMatrix()
	{
		vec3 scale{ mWorldBoundingSphere.radius, mWorldBoundingSphere.radius, mWorldBoundingSphere.radius };

		mRenderObject.modelMatrix = MathEngine::Scale4x4(scale) * MathEngine::Translate(mWorldBoundingSphere.center);
	}

	void BoundingSphere::TransformBoundingVolume(const mat4& model)
	{
		TransformSphere(mWorldBoundingSphere, mLocalBoundingSphere, model);
	}
}