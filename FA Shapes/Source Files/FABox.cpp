#include "FABox.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace FAShapes
{
	Box::Box(FAColor::Color color) : mColor{ color }, mX{ 1.0f, 0.0f, 0.0f }, mY{ 0.0f, 1.0f, 0.0f }, mZ{ 0.0f, 0.0f, 1.0f }
	{
		createVertices();
		createTriangles();
		createNormals();

		mLocalVertices[0].color = mColor;
		mLocalVertices[1].color = mColor;
		mLocalVertices[2].color = mColor;
		mLocalVertices[3].color = mColor;
		mLocalVertices[4].color = mColor;
		mLocalVertices[5].color = mColor;
		mLocalVertices[6].color = mColor;
		mLocalVertices[7].color = mColor;
	}

	//strores the local vertices of box
	void Box::createVertices()
	{
		//local coordinates for each vertex of the box
		mLocalVertices[0].position = FAMath::Vector3D(-1.0f, 1.0f, -1.0f);
		mLocalVertices[1].position = FAMath::Vector3D(1.0f, 1.0f, -1.0f);
		mLocalVertices[2].position = FAMath::Vector3D(1.0f, -1.0f, -1.0f);
		mLocalVertices[3].position = FAMath::Vector3D(-1.0f, -1.0f, -1.0f);
		mLocalVertices[4].position = FAMath::Vector3D(-1.0f, 1.0f, 1.0f);
		mLocalVertices[5].position = FAMath::Vector3D(1.0f, 1.0f, 1.0f);
		mLocalVertices[6].position = FAMath::Vector3D(1.0f, -1.0f, 1.0f);
		mLocalVertices[7].position = FAMath::Vector3D(-1.0f, -1.0f, 1.0f);
	}

	//creates the triangles from the local vertices
	void Box::createTriangles()
	{
		//the indices of the vertices that make up each face of the box
		unsigned int triangleIndex{ 0 };
		quad(triangleIndex, 0, 1, 2, 3); //back face
		quad(triangleIndex, 5, 4, 7, 6); //front face
		quad(triangleIndex, 4, 5, 1, 0); //top face
		quad(triangleIndex, 3, 2, 6, 7); //bottom face
		quad(triangleIndex, 4, 0, 3, 7); //left face
		quad(triangleIndex, 1, 5, 6, 2); //right face
	}

	//creates the normals from the triangles
	void Box::createNormals()
	{
		//Each vertex has a normal assoicated with it. Each triangle has a normal. A vertex can be a part of multiple triangles.
		//Get the average of all the triangle normals each vertex is a part of.
		//For each vertex we sum all of the triangle normals it is a part of.
		std::unordered_map<unsigned int, std::pair<unsigned int, FAMath::Vector3D>> normals;
		for (int i = 0; i < NUMOFTRIANGLES; ++i)
		{
			FAMath::Vector3D norm(mTriangles[i].getNormal());
			normals[mTriangles[i].getP0Index()].first++;
			normals[mTriangles[i].getP0Index()].second += norm;

			normals[mTriangles[i].getP1Index()].first++;
			normals[mTriangles[i].getP1Index()].second += norm;

			normals[mTriangles[i].getP2Index()].first++;
			normals[mTriangles[i].getP2Index()].second += norm;
		}

		//Take the average to get the normal for that vertex.
		for (int i = 0; i < NUMOFVERTICES; ++i)
		{
			mLocalVertices[i].normal = normals[i].second / normals[i].first;
		}
	}

	/*stores the indices of the vertices that make up each triangle.
	* A box is made up of 6 faces.
	* A face has two triangles.
	* a		   b
	* o--------o
	* |	 \	   |
	  |   \	   |
	* |	   \   |
	  |		\  |
	* o--------o
	* d		   c
	* A line from a to c shows the two triangles abc and acd.
	*/
	void Box::quad(unsigned int& index, unsigned int a, unsigned int b, unsigned int c, unsigned int d)
	{
		mTriangles[index] = Triangle(mLocalVertices, a, b, c);
		++index;
		mTriangles[index] = Triangle(mLocalVertices, a, c, d);
		++index;
	}

	FAMath::Vector3D Box::getCenter() const
	{
		return mCenter;
	}

	FAMath::Vector3D Box::getXAxis() const
	{
		return mX;
	}

	FAMath::Vector3D Box::getYAxis() const
	{
		return mY;
	}

	FAMath::Vector3D Box::getZAxis() const
	{
		return mZ;
	}

	Vertex* Box::getVertexList()
	{
		return mLocalVertices;
	}

	Triangle* Box::getTriangleList()
	{
		return mTriangles;
	}

	const Vertex* Box::getVertexList() const
	{
		return mLocalVertices;
	}

	const Triangle* Box::getTriangleList() const
	{
		return mTriangles;
	}

	const Triangle& Box::getTriangle(unsigned int index) const
	{
		if (index >= NUMOFTRIANGLES)
			throw std::out_of_range("In the getTriangle function, index is out of range");

		return mTriangles[index];
	}

	FAColor::Color Box::getColor() const
	{
		return mColor;
	}

	FAMath::Matrix4x4 Box::getScaleMatrix() const
	{
		return mScale;
	}

	FAMath::Matrix4x4 Box::getLocalToWorldMatrix() const
	{
		return mLocalToWorld;
	}

	DrawArguments Box::getDrawArguments() const
	{
		return mBoxDrawArguments;
	}

	void Box::setColor(const FAColor::Color& color)
	{
		mColor = color;

		mLocalVertices[0].color = mColor;
		mLocalVertices[1].color = mColor;
		mLocalVertices[2].color = mColor;
		mLocalVertices[3].color = mColor;
		mLocalVertices[4].color = mColor;
		mLocalVertices[5].color = mColor;
		mLocalVertices[6].color = mColor;
		mLocalVertices[7].color = mColor;
	}

	void Box::setColor(float r, float g, float b, float a)
	{
		mColor.setRed(r);
		mColor.setGreen(g);
		mColor.setBlue(b);
		mColor.setAlpha(a);

		mLocalVertices[0].color = mColor;
		mLocalVertices[1].color = mColor;
		mLocalVertices[2].color = mColor;
		mLocalVertices[3].color = mColor;
		mLocalVertices[4].color = mColor;
		mLocalVertices[5].color = mColor;
		mLocalVertices[6].color = mColor;
		mLocalVertices[7].color = mColor;
	}

	void Box::setScaleMatrix(const FAMath::Matrix4x4& scale)
	{
		mScale = scale;
	}

	void Box::setCenter(const FAMath::Vector3D& center)
	{
		mCenter = center;
	}

	void Box::setDrawArguments(const DrawArguments& boxDrawArgs)
	{
		mBoxDrawArguments = boxDrawArgs;
	}

	void Box::setDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex, 
		int indexOfFirstVertex, int indexOfConstantData)
	{
		mBoxDrawArguments = { indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
	}

	void Box::rotateAxes(const FAMath::Matrix4x4& rot)
	{
		FAMath::Vector4D x(mX, 1.0f);
		FAMath::Vector4D y(mY, 1.0f);
		FAMath::Vector4D z(mZ, 1.0f);

		x = x * rot;
		y = y * rot;
		z = z * rot;

		mX = FAMath::Vector3D(x.x(), x.y(), x.z());
		mY = FAMath::Vector3D(y.x(), y.y(), y.z());
		mZ = FAMath::Vector3D(z.x(), z.y(), z.z());

		//orthonormalize the boxs local axes.
		orthonormalize(mX, mY, mZ);
	}

	void Box::rotateAxes(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Vector4D x(mX, 1.0f);
		FAMath::Vector4D y(mY, 1.0f);
		FAMath::Vector4D z(mZ, 1.0f);

		FAMath::Matrix4x4 rot{ quaternionRotationMatrixRow(rotQuaternion) };
		x = x * rot;
		y = y * rot;
		z = z * rot;

		mX = FAMath::Vector3D(x.x(), x.y(), x.z());
		mY = FAMath::Vector3D(y.x(), y.y(), y.z());
		mZ = FAMath::Vector3D(z.x(), z.y(), z.z());

		//orthonormalize the boxs local axes.
		orthonormalize(mX, mY, mZ);
	}

	void Box::rotateAxes(float angle, const FAMath::Vector3D axis)
	{
		FAMath::Vector4D x(mX, 1.0f);
		FAMath::Vector4D y(mY, 1.0f);
		FAMath::Vector4D z(mZ, 1.0f);

		FAMath::Matrix4x4 rot{ quaternionRotationMatrixRow(rotationQuaternion(angle, axis)) };

		x = x * rot;
		y = y * rot;
		z = z * rot;

		mX = FAMath::Vector3D(x.x(), x.y(), x.z());
		mY = FAMath::Vector3D(y.x(), y.y(), y.z());
		mZ = FAMath::Vector3D(z.x(), z.y(), z.z());

		//orthonormalize the boxs local axes.
		orthonormalize(mX, mY, mZ);
	}

	void Box::rotateCenter(const FAMath::Matrix4x4& rot)
	{
		FAMath::Vector4D center(mCenter, 1.0f);

		center = center * rot;

		mCenter = FAMath::Vector3D(center.x(), center.y(), center.z());
	}

	void Box::rotateCenter(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Vector4D center(mCenter, 1.0f);

		FAMath::Matrix4x4 rot{ quaternionRotationMatrixRow(rotQuaternion) };
		center = center * rot;

		mCenter = FAMath::Vector3D(center.x(), center.y(), center.z());
	}

	void Box::rotateCenter(float angle, const FAMath::Vector3D axis)
	{
		FAMath::Vector4D center(mCenter, 1.0f);

		FAMath::Matrix4x4 rot{ quaternionRotationMatrixRow(rotationQuaternion(angle, axis)) };
		center = center * rot;

		mCenter = FAMath::Vector3D(center.x(), center.y(), center.z());
	}

	void Box::translateCenter(float x, float y, float z)
	{
		mCenter.x() += x;
		mCenter.y() += y;
		mCenter.z() += z;
	}

	void Box::translateCenter(FAMath::Vector3D v)
	{
		mCenter += v;
	}

	//multiplies the scale, local rotation, and translation in that order.
	//S * LR * T.
	void Box::updateLocalToWorldMatrix()
	{
		FAMath::Matrix4x4 localRotation;
		localRotation.setRow(0, FAMath::Vector4D(mX));
		localRotation.setRow(1, FAMath::Vector4D(mY));
		localRotation.setRow(2, FAMath::Vector4D(mZ));

		FAMath::Matrix4x4 translation{ translate(FAMath::Matrix4x4(), mCenter.x(), mCenter.y(), mCenter.z()) };

		mLocalToWorld = mScale * localRotation * translation;
	}
}