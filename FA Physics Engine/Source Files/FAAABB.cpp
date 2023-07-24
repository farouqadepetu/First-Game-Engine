#include "FAAABB.h"

namespace FACollisions
{
	AABB CreateAABB(const std::vector<FAShapes::Vertex>& vertexList)
	{
		AABB result;

		result.min = vertexList[0].position;
		result.max = vertexList[0].position;
		for (auto& i : vertexList)
		{
			if (i.position.GetX() < result.min.GetX())
			{
				result.min.SetX(i.position.GetX());
			}
			if (i.position.GetX() > result.max.GetX())
			{
				result.max.SetX(i.position.GetX());
			}

			if (i.position.GetY() < result.min.GetY())
			{
				result.min.SetY(i.position.GetY());
			}
			if (i.position.GetY() > result.max.GetY())
			{
				result.max.SetY(i.position.GetY());
			}

			if (i.position.GetZ() < result.min.GetZ())
			{
				result.min.SetZ(i.position.GetZ());
			}
			if (i.position.GetZ() > result.max.GetZ())
			{
				result.max.SetZ(i.position.GetZ());
			}
		}

		return result;
	}
}