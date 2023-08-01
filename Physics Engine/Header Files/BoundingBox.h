#pragma once

#include "BoundingVolume.h"
#include "Vertex.h"

namespace PhysicsEngine
{
	/**brief Structure for an axis-aligned bounding box.
	* Uses the min-max representation.
	*/
	struct AABB
	{
		vec3 min;
		vec3 max;
	};

	/**brief Initializes the specified AABB with the specified min and max values;
	*/
	void InitializeAABB(AABB& a, vec3 min, vec3 max);

	/**brief Computes the properties of a AABB from the vertices of an object.
	*/
	void ComputeAABB(AABB& aabb, const std::vector<ShapesEngine::Vertex>& vertices);

	/**brief Transforms the localAABB to world space by finding the extents and returning the resultant AABB.
	* The transformation is done using vector-matrix multiplcation. The localAABB is treated as a row vector.
	*/
	void TransformAABB(AABB& worldAABB, const AABB& localAABB, const mat4& model);

	/**brief Returns true if the specified AABBs are intersecting, false otherwise.
	*/
	bool TestIntersection(const AABB& a, const AABB& b);


	/** @class BoundingBox ""
	*	@brief This class is used to bound an object using an axis-aligned bounding box and also for rendering it.
	*/
	class BoundingBox : public BoundingVolumeAbstract
	{
	public:
		/**brief Default constructor.
		*/
		BoundingBox();

		/**brief Initializes the properties of the BoundingBox.
		*/
		BoundingBox(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color);

		/**brief Initializes the properties of the BoundingBox.
		*/
		void InitializeBoundingBox(const std::vector<ShapesEngine::Vertex>& vertices, const RenderingEngine::Color& color);

		/**@brief Updates the BoundingBox model matrix.
		*/
		void UpdateModelMatrix() override;

		/**@brief Transforms the BoundingBox from local space to world space.
		*/
		void TransformBoundingVolume(const mat4& model) override;

	private:
		AABB mLocalAABB;
		AABB mWorldAABB;
	};
}