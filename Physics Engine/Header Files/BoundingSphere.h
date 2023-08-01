#pragma once

#include "BoundingVolume.h"
#include "Color.h"
#include "Vertex.h"
#include <vector>

namespace PhysicsEngine
{
	struct Sphere
	{
		vec3 center;
		float radius{ 1.0f };
	};

	/**brief Initializes the properties of a sphere.
	*/
	void InitalizeSphere(Sphere& sphere, const vec3& center, float radius);

	/**brief Computes the properties of a sphere from the vertices of an object using Ritter's method.
	*/
	void ComputeSphere(Sphere& sphere, const std::vector<ShapesEngine::Vertex>& vertices);

	/**brief Transforms the sphere from local space to world space using a row-major transformation matrix.
	*/
	void TransformSphere(Sphere& worldSphere, const Sphere& localSphere, const mat4& model);

	/**brief Returns true if the two spheres are intersecting, false otherwise.
	*/
	bool TestIntersection(const Sphere& a, const Sphere& b);

	/** @class BoundingSphere ""
	*	@brief This class is used to bound an object using a sphere and also for rendering it.
	*/
	class BoundingSphere : public BoundingVolumeAbstract
	{
	public:
		/**brief Default constructor.
		*/
		BoundingSphere();

		/**brief Initializes the properties of the BoundingSphere.
		*/
		BoundingSphere(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color);

		/**brief Initializes the properties of the BoundingSphere.
		*/
		void InitializeBoundingSphere(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color);

		/**@brief Updates the BoundingSpheres model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Transforms the BoundingSphere from local space to world space.
		*/
		void TransformBoundingVolume(const mat4& model) override;

	private:
		Sphere mLocalBoundingSphere;
		Sphere mWorldBoundingSphere;
	};
}