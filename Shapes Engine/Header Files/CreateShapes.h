#pragma once

#include "Triangle.h"
#include <vector>

namespace ShapesEngine
{
	/**@brief Creates the vertices of a unit box and connects them using triangles.
	*
	* Also computes the normal for each vertex.
	*/
	void CreateBox(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles);

	/**@brief Creates the vertices of a unit cone and connects them using triangles.
	*
	* Also computes the normal for each vertex.
	* Uses the UV-method to create the vertices of the cone.
	*/
	void CreateCone(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles,
		unsigned int numVerticesPerCircle = 20, unsigned int numCircles = 20);

	/**@brief Creates the vertices of a unit cone and connects them using triangles.
	*
	* Also computes the normal for each vertex./n
	* Uses the UV-method to create the vertices of the cylinder.
	*/
	void CreateCylinder(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles,
		unsigned int numVerticesPerCircle = 20, unsigned int numCircles = 20);

	/**@brief Creates the vertices of a unit sphere and connects them using triangles.
	*
	* Also computes the normal for each vertex./n
	* Uses the UV-method to create the vertices of the sphere.
	*/
	void CreateSphere(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles,
		unsigned int numVerticesPerCircle = 20, unsigned int numCircles = 20);

	/**@brief Creates the vertices of a unit pyramid and connects them using triangles.
	*
	* Also computes the normal for each vertex.
	*/
	void CreatePyramid(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles);
}