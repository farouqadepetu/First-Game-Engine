#pragma once

/** @file FAShapesUtility.h
*	@brief File has utility functions.
*/

#include "FATriangle.h"
#include <vector>

/** @namespace FAShapes
*	@brief Has classes that are used for creating 3D shapes.
*/
namespace FAShapes
{

	/**@brief Creates the vertices of a unit box and connects them using triangles.
	* 
	* Also computes the normal for each vertex.
	*/
	void CreateBox(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles);

	/**@brief Creates the vertices of a unit cone and connects them using triangles.
	* 
	* Also computes the normal for each vertex.
	* Uses the UV-method to create the vertices of the cone.
	*/
	void CreateCone(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles,
		unsigned int numVerticesPerCircle = 20, unsigned int numCircles = 20);

	/**@brief Creates the vertices of a unit cone and connects them using triangles.
	*
	* Also computes the normal for each vertex./n
	* Uses the UV-method to create the vertices of the cylinder.
	*/
	void CreateCylinder(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles,
		unsigned int numVerticesPerCircle = 20, unsigned int numCircles = 20);

	/**@brief Creates the vertices of a unit sphere and connects them using triangles.
	*
	* Also computes the normal for each vertex./n
	* Uses the UV-method to create the vertices of the sphere.
	*/
	void CreateSphere(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles,
		unsigned int numVerticesPerCircle = 20, unsigned int numCircles = 20);

	/**@brief Creates the vertices of a unit pyramid and connects them using triangles.
	*
	* Also computes the normal for each vertex.
	*/
	void CreatePyramid(std::vector<FAShapes::Vertex>& vertices, std::vector<FAShapes::Triangle>& triangles);
}