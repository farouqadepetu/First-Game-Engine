#pragma once

//#include "FAVector3D.h"
#include "FAMathEngine.h"
#include "FAColor.h"

namespace FAShapes
{
	/** @struct DrawArguments ""
	*	@brief Has all the data that are used as parameters to draw an object.
	*/
	struct DrawArguments
	{
		unsigned int indexCount;
		unsigned int locationOfFirstIndex;
		int indexOfFirstVertex;
		int indexOfConstantData;
	};


	/** @struct Vertex ""
	*	@brief Has all the data that describes a vertex.
	*/
	struct Vertex
	{
		FAMath::Vector3D position;
		FAColor::Color color;
		FAMath::Vector3D normal;
	};
}