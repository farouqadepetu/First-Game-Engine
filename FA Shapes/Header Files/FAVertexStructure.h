#pragma once

#include "FAMathEngine.h"

namespace FAShapes
{
	/** @struct Vertex ""
	*	@brief Data that describes a vertex.
	*/
	struct Vertex
	{
		FAMath::Vector4D position;
		FAMath::Vector4D normal;
		FAMath::Vector2D texCoords;
	};
}