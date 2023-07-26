#pragma once

#include "MathEngine.h"
#include "Vertex.h"
#include <vector>

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

	/**brief Returns an AABB to bound the specified object.
	* @param[in] vertexList The vertex list of the object to bound by the AABB.
	*/
	AABB CreateAABB(const std::vector<ShapesEngine::Vertex>& vertexList);

}