#include "Triangle.h"

namespace ShapesEngine
{
	vec3 ComputeNormal(const Triangle& triangle)
	{
		vec3 p01(triangle.vertexList[triangle.p1].position - triangle.vertexList[triangle.p0].position);
		vec3 p02(triangle.vertexList[triangle.p2].position - triangle.vertexList[triangle.p0].position);

		return MathEngine::CrossProduct(p01, p02);
	}

	vec3 ComputeCenter(const Triangle& triangle)
	{
		vec3 p0(triangle.vertexList[triangle.p0].position);
		vec3 p1(triangle.vertexList[triangle.p1].position);
		vec3 p2(triangle.vertexList[triangle.p2].position);

		float x{ (p0.x + p1.x + p2.x) / 3.0f };
		float y{ (p0.y + p1.y + p2.y) / 3.0f };
		float z{ (p0.z + p1.z + p2.z) / 3.0f };

		return vec3{ x, y, z };
	}

	/* A quad has two triangles.
	* a		   b
	* o--------o
	* |  \	   |
	* |   \	   |
	* |    \   |
	* |		\  |
	* o--------o
	* d		   c
	* A line from a to c shows the two triangles abc and acd.
	*/
	void Quad(unsigned int a, unsigned int b, unsigned int c, unsigned int d, std::vector<Triangle>& triangles, Vertex* vertices)
	{
		triangles.push_back(Triangle{ vertices, a, b, c });
		triangles.push_back(Triangle{ vertices, a, c, d });
	}
}