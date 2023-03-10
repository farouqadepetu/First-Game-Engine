#include "FATriangle.h"

namespace FAShapes
{
	Triangle::Triangle(FAMath::Vector3D* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
		: mVertexList{ vertexList }, mIndexList{ p0Index, p1Index, p2Index }
	{
		if mVertexList != nullptr)
		{
			if (isColinear())
				throw std::runtime_error("The triangle vertices are colinear");
		}
	}

	FAMath::Vector3D Triangle::getP0()
	{
		return mVertexList[mIndexList[0]];
	}

	FAMath::Vector3D Triangle::getP1()
	{
		return mVertexList[mIndexList[1]];
	}

	FAMath::Vector3D Triangle::getP2()
	{
		return mVertexList[mIndexList[2]];
	}

	unsigned int Triangle::getP0Index()
	{
		return mIndexList[0];
	}

	unsigned int Triangle::getP1Index()
	{
		return mIndexList[1];
	}

	unsigned int Triangle::getP2Index()
	{
		return mIndexList[2];
	}

	//Normal of a traingle is (p1 - p0) x (p2 - p0)
	FAMath::Vector3D Triangle::getNormal()
	{
		FAMath::Vector3D p01{ mVertexList[mIndexList[1]] - mVertexList[mIndexList[0]] };
		FAMath::Vector3D p02{ mVertexList[mIndexList[2]] - mVertexList[mIndexList[0]] };

		return crossProduct(p01, p02);
	}

	//Center of a triangle is 
	//< (x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3 >
	FAMath::Vector3D Triangle::getCenter()
	{
		FAMath::Vector3D p0{ mVertexList[mIndexList[0]] };
		FAMath::Vector3D p1{ mVertexList[mIndexList[1]] };
		FAMath::Vector3D p2{ mVertexList[mIndexList[2]] };

		float x{ (p0.x() + p1.x() + p2.x()) / 3.0f };
		float y{ (p0.y() + p1.y() + p2.y()) / 3.0f };
		float z{ (p0.z() + p1.z() + p2.z()) / 3.0f };

		return FAMath::Vector3D(x, y, z);
	}

	void Triangle::setVertexList(FAMath::Vector3D* vertexList)
	{
		mVertexList = vertexList;
	}

	void Triangle::setTriangleIndices(unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
	{
		mIndexList[0] = p0Index;
		mIndexList[1] = p1Index;
		mIndexList[2] = p2Index;

		if (isColinear())
			throw std::runtime_error("The triangle vertices are colinear");
	}

	void Triangle::setTriangle(FAMath::Vector3D* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index)
	{
		mVertexList = vertexList;
		mIndexList[0] = p0Index;
		mIndexList[1] = p1Index;
		mIndexList[2] = p2Index;

		if (isColinear())
			throw std::runtime_error("The triangle vertices are colinear");
	}

	//If the area of th triangle formed by the 3 points isn't zero then they aren't colinear.
	//The area of a triangle is |((p1 - p0) x (p2 - p0))| / 2
	bool Triangle::isColinear()
	{
		FAMath::Vector3D p01{ mVertexList[mIndexList[1]] - mVertexList[mIndexList[0]] };
		FAMath::Vector3D p02{ mVertexList[mIndexList[2]] - mVertexList[mIndexList[0]] };

		float area = length(crossProduct(p01, p02)) / 2.0f;

		if (FAMath::compareFloats(area, 0.0f, 1e-6))
			return true;
		
		return false;
	}
}