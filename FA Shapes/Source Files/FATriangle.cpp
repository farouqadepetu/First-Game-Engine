#include "FATriangle.h"
#include <stdexcept>
#include <sstream>

namespace FAShapes
{
	Triangle::Triangle(FAShapes::Vertex* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
		: mVertexList{ vertexList }, mIndexList{ p0Index, p1Index, p2Index }
	{}

	const FAShapes::Vertex& Triangle::GetP0() const
	{
		return mVertexList[mIndexList[0]];
	}

	const FAShapes::Vertex& Triangle::GetP1() const
	{
		return mVertexList[mIndexList[1]];
	}

	const FAShapes::Vertex& Triangle::GetP2() const
	{
		return mVertexList[mIndexList[2]];
	}

	unsigned int Triangle::GetP0Index() const
	{
		return mIndexList[0];
	}

	unsigned int Triangle::GetP1Index() const
	{
		return mIndexList[1];
	}

	unsigned int Triangle::GetP2Index() const
	{
		return mIndexList[2];
	}

	//Normal of a traingle is (p1 - p0) x (p2 - p0)
	FAMath::Vector4D Triangle::GetNormal() const
	{
		FAMath::Vector4D p01{ mVertexList[mIndexList[1]].position - mVertexList[mIndexList[0]].position };
		FAMath::Vector4D p02{ mVertexList[mIndexList[2]].position - mVertexList[mIndexList[0]].position };

		FAMath::Vector3D tempP01(p01.GetX(), p01.GetY(), p01.GetZ());
		FAMath::Vector3D tempP02(p02.GetX(), p02.GetY(), p02.GetZ());
		FAMath::Vector3D result(Norm(CrossProduct(tempP01, tempP02)));

		return FAMath::Vector4D(result.GetX(), result.GetY(), result.GetZ(), 0.0f);
	}

	//Center of a triangle is 
	//< (x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3 >
	FAMath::Vector4D Triangle::GetCenter() const
	{
		FAMath::Vector4D p0{ mVertexList[mIndexList[0]].position };
		FAMath::Vector4D p1{ mVertexList[mIndexList[1]].position };
		FAMath::Vector4D p2{ mVertexList[mIndexList[2]].position };

		float x{ (p0.GetX() + p1.GetX() + p2.GetX()) / 3.0f };
		float y{ (p0.GetY() + p1.GetY() + p2.GetY()) / 3.0f };
		float z{ (p0.GetZ() + p1.GetZ() + p2.GetZ()) / 3.0f };

		return FAMath::Vector4D(x, y, z, 1.0f);
	}

	void Triangle::SetVertexList(FAShapes::Vertex* vertexList)
	{
		mVertexList = vertexList;
	}

	void Triangle::SetP0Index(unsigned int index)
	{
		mIndexList[0] = index;
	}

	void Triangle::SetP1Index(unsigned int index)
	{
		mIndexList[1] = index;
	}

	void Triangle::SetP2Index(unsigned int index)
	{
		mIndexList[2] = index;
	}

	void Triangle::SetTriangleIndices(unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
	{
		mIndexList[0] = p0Index;
		mIndexList[1] = p1Index;
		mIndexList[2] = p2Index;
	}

	void Triangle::SetTriangle(FAShapes::Vertex* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
	{
		mVertexList = vertexList;
		mIndexList[0] = p0Index;
		mIndexList[1] = p1Index;
		mIndexList[2] = p2Index;
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
	void Quad(unsigned int a, unsigned int b, unsigned int c, unsigned int d, std::vector<Triangle>& triangles, FAShapes::Vertex* vertices)
	{
		triangles.push_back(Triangle(vertices, a, b, c));
		triangles.push_back(Triangle(vertices, a, c, d));
	}
}