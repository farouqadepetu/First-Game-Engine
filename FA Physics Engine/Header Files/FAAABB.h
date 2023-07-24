#pragma once

#include "FAMathEngine.h"
#include "FAShapesUtility.h"
#include <vector>

namespace FACollisions
{
	/**brief Structure for an axis-aligned bounding box.
	* Uses the min-max representation.
	*/
	struct AABB
	{
		FAMath::Vector4D min;
		FAMath::Vector4D max;
	};

	/**brief Returns an AABB to bound the specified object.
	* @param[in] vertexList The vertex list of the object to bound by the AABB.
	*/
	AABB CreateAABB(const std::vector<FAShapes::Vertex>& vertexList);

}