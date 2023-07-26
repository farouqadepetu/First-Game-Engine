#include "AABB.h"

namespace PhysicsEngine
{
	AABB CreateAABB(const std::vector<ShapesEngine::Vertex>& vertexList)
	{
		AABB result;

		result.min = vertexList[0].position;
		result.max = vertexList[0].position;
		for (auto& i : vertexList)
		{
			if (i.position.x < result.min.x)
			{
				result.min.x = i.position.x;
			}
			if (i.position.x > result.max.x)
			{
				result.max.x = i.position.x;
			}

			if (i.position.y < result.min.y)
			{
				result.min.y = i.position.y;
			}
			if (i.position.y > result.max.y)
			{
				result.max.y = i.position.y;
			}

			if (i.position.z < result.min.z)
			{
				result.min.z = i.position.z;
			}
			if (i.position.z > result.max.z)
			{
				result.max.z = i.position.z;
			}
		}

		return result;
	}
}