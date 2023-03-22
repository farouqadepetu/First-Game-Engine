#include "FATriangle.h"
#include <stdexcept>

namespace FAShapes
{
	Triangle::Triangle(Vertex* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
		: mVertexList{ vertexList }, mIndexList{ p0Index, p1Index, p2Index }
	{
		if (mVertexList != nullptr)
		{
			if (IsColinear())
				throw std::runtime_error("The triangle vertices are colinear");
		}
	}

	const Vertex& Triangle::GetP0() const
	{
		return mVertexList[mIndexList[0]];
	}

	const Vertex& Triangle::GetP1() const
	{
		return mVertexList[mIndexList[1]];
	}

	const Vertex& Triangle::GetP2() const
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
	const FAMath::Vector3D& Triangle::GetNormal() const
	{
		FAMath::Vector3D p01{ mVertexList[mIndexList[1]].position - mVertexList[mIndexList[0]].position };
		FAMath::Vector3D p02{ mVertexList[mIndexList[2]].position - mVertexList[mIndexList[0]].position };

		return Norm(CrossProduct(p01, p02));
	}

	//Center of a triangle is 
	//< (x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3 >
	const FAMath::Vector3D &Triangle::GetCenter() const
	{
		FAMath::Vector3D p0{ mVertexList[mIndexList[0]].position };
		FAMath::Vector3D p1{ mVertexList[mIndexList[1]].position };
		FAMath::Vector3D p2{ mVertexList[mIndexList[2]].position };

		float x{ (p0.GetX() + p1.GetX() + p2.GetX()) / 3.0f };
		float y{ (p0.GetY() + p1.GetY() + p2.GetY()) / 3.0f };
		float z{ (p0.GetZ() + p1.GetZ() + p2.GetZ()) / 3.0f };

		return FAMath::Vector3D(x, y, z);
	}

	void Triangle::SetVertexList(Vertex* vertexList)
	{
		mVertexList = vertexList;
	}

	void Triangle::SetTriangleIndices(unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
	{
		mIndexList[0] = p0Index;
		mIndexList[1] = p1Index;
		mIndexList[2] = p2Index;

		if (IsColinear())
			throw std::runtime_error("The triangle vertices are colinear");
	}

	void Triangle::SetTriangle(Vertex* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
	{
		mVertexList = vertexList;
		mIndexList[0] = p0Index;
		mIndexList[1] = p1Index;
		mIndexList[2] = p2Index;

		if (IsColinear())
			throw std::runtime_error("The triangle vertices are colinear");
	}

	//If the area of th triangle formed by the 3 points isn't zero then they aren't colinear.
	//The area of a triangle is |((p1 - p0) x (p2 - p0))| / 2
	bool Triangle::IsColinear()
	{
		FAMath::Vector3D p01{ mVertexList[mIndexList[1]].position - mVertexList[mIndexList[0]].position };
		FAMath::Vector3D p02{ mVertexList[mIndexList[2]].position - mVertexList[mIndexList[0]].position };

		float area = Length(CrossProduct(p01, p02)) / 2.0f;

		if (FAMath::CompareFloats(area, 0.0f, 1e-6f))
			return true;
		
		return false;
	}
}