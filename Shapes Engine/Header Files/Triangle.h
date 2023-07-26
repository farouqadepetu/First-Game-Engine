#pragma once

#include <vector>
#include "Vertex.h"

namespace ShapesEngine
{
	/** @struct Triangle ""
	*	@brief The struct stores a pointer to a vertex list and indices to the vertices of the triangle.
	*/
	struct Triangle
	{
		Vertex* vertexList; //pointer to a vertex list
		unsigned int p0;
		unsigned int p1;
		unsigned int p2;
	};

	/**@brief Returns the normal of the triangle.
	*/
	vec3 ComputeNormal(const Triangle& triangle);

	/**@brief Returns the center of the triangle.
	*/
	vec3 ComputeCenter(const Triangle& triangle);

	/**@brief Stores the indices of the vertices of the triangles that make up a shape.
	*/
	void Quad(unsigned int a, unsigned int b, unsigned int c, unsigned int d, std::vector<Triangle>& triangles, Vertex* vertices);
}