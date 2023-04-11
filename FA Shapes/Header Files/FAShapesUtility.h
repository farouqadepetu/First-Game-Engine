#pragma once

/** @file FAShapesUtility.h
*	@brief File has structures DrawArguments and Vertex under the namespace FAShapes.
*/


#include "FAMathEngine.h"
#include "FAColor.h"

/** @namespace FAShapes
*	@brief Has classes that are used for creating 3D shapes.
*/
namespace FAShapes
{
	/** @struct DrawArguments ""
	*	@brief Data that are used as parameters to draw an object.
	*/
	struct DrawArguments
	{
		unsigned int indexCount;
		unsigned int locationOfFirstIndex;
		int indexOfFirstVertex;
		unsigned int indexOfConstantData;
	};


	/** @struct Vertex ""
	*	@brief Data that describes a vertex.
	*/
	struct Vertex
	{
		FAMath::Vector3D position;
		FAColor::Color color;
		FAMath::Vector3D normal;
	};
}