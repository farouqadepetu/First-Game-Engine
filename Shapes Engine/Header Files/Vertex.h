#pragma once

#include "MathEngine.h"

namespace ShapesEngine
{
	/** @struct Vertex ""
	*	@brief Data that describes a vertex.
	*/
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 texCoords;
	};
}