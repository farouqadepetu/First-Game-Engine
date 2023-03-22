#include "FABox.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace FAShapes
{
	Box::Box(FAColor::Color color) : mColor{ color }, mX{ 1.0f, 0.0f, 0.0f }, mY{ 0.0f, 1.0f, 0.0f }, mZ{ 0.0f, 0.0f, 1.0f }
	{
		mCreateVertices();
		mCreateTriangles();
		mCreateNormals();

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
	void Box::mCreateVertices()
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
	void Box::mCreateTriangles()
	{
		//the indices of the vertices that make up each face of the box
		unsigned int triangleIndex{ 0 };
		mQuad(triangleIndex, 0, 1, 2, 3); //back face
		mQuad(triangleIndex, 5, 4, 7, 6); //front face
		mQuad(triangleIndex, 4, 5, 1, 0); //top face
		mQuad(triangleIndex, 3, 2, 6, 7); //bottom face
		mQuad(triangleIndex, 4, 0, 3, 7); //left face
		mQuad(triangleIndex, 1, 5, 6, 2); //right face
	}

	//creates the normals from the triangles
	void Box::mCreateNormals()
	{
		//Each vertex has a normal assoicated with it. Each triangle has a normal. A vertex can be a part of multiple triangles.
		//Get the average of all the triangle normals each vertex is a part of.
		//For each vertex we sum all of the triangle normals it is a part of.
		std::unordered_map<unsigned int, std::pair<unsigned int, FAMath::Vector3D>> normals;
		for (int i = 0; i < NUM_OF_TRIANGLES; ++i)
		{
			FAMath::Vector3D norm(mTriangles[i].GetNormal());
			normals[mTriangles[i].GetP0Index()].first++;
			normals[mTriangles[i].GetP0Index()].second += norm;

			normals[mTriangles[i].GetP1Index()].first++;
			normals[mTriangles[i].GetP1Index()].second += norm;

			normals[mTriangles[i].GetP2Index()].first++;
			normals[mTriangles[i].GetP2Index()].second += norm;
		}

		//Take the average to get the normal for that vertex.
		for (int i = 0; i < NUM_OF_VERTICES; ++i)
		{
			mLocalVertices[i].normal = normals[i].second / (float)normals[i].first;
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
	void Box::mQuad(unsigned int& index, unsigned int a, unsigned int b, unsigned int c, unsigned int d)
	{
		mTriangles[index] = Triangle(mLocalVertices, a, b, c);
		++index;
		mTriangles[index] = Triangle(mLocalVertices, a, c, d);
		++index;
	}

	const FAMath::Vector3D& Box::GetCenter() const
	{
		return mCenter;
	}

	const FAMath::Vector3D& Box::GetXAxis() const
	{
		return mX;
	}

	const FAMath::Vector3D& Box::GetYAxis() const
	{
		return mY;
	}

	const FAMath::Vector3D& Box::GetZAxis() const
	{
		return mZ;
	}

	Vertex* Box::GetVertexList()
	{
		return mLocalVertices;
	}

	Triangle* Box::GetTriangleList()
	{
		return mTriangles;
	}

	const Vertex* Box::GetVertexList() const
	{
		return mLocalVertices;
	}

	const Triangle* Box::GetTriangleList() const
	{
		return mTriangles;
	}

	const Triangle& Box::GetTriangle(unsigned int index) const
	{
		if (index >= NUM_OF_TRIANGLES)
			throw std::out_of_range("In the getTriangle function, index is out of range");

		return mTriangles[index];
	}

	const FAColor::Color& Box::GetColor() const
	{
		return mColor;
	}

	const FAMath::Matrix4x4& Box::GetScaleMatrix() const
	{
		return mScale;
	}

	const FAMath::Matrix4x4& Box::GetLocalToWorldMatrix() const
	{
		return mLocalToWorld;
	}

	const DrawArguments& Box::GetDrawArguments() const
	{
		return mBoxDrawArguments;
	}

	void Box::SetColor(const FAColor::Color& color)
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

	void Box::SetColor(float r, float g, float b, float a)
	{
		mColor.SetRed(r);
		mColor.SetGreen(g);
		mColor.SetBlue(b);
		mColor.SetAlpha(a);

		mLocalVertices[0].color = mColor;
		mLocalVertices[1].color = mColor;
		mLocalVertices[2].color = mColor;
		mLocalVertices[3].color = mColor;
		mLocalVertices[4].color = mColor;
		mLocalVertices[5].color = mColor;
		mLocalVertices[6].color = mColor;
		mLocalVertices[7].color = mColor;
	}

	void Box::SetScaleMatrix(const FAMath::Matrix4x4& scale)
	{
		mScale = scale;
	}

	void Box::SetCenter(const FAMath::Vector3D& center)
	{
		mCenter = center;
	}

	void Box::SetDrawArguments(const DrawArguments& boxDrawArgs)
	{
		mBoxDrawArguments = boxDrawArgs;
	}

	void Box::SetDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex, 
		int indexOfFirstVertex, int indexOfConstantData)
	{
		mBoxDrawArguments = { indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
	}

	void Box::RotateAxes(const FAMath::Matrix4x4& rot)
	{
		FAMath::Vector4D x(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f);
		FAMath::Vector4D y(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f);
		FAMath::Vector4D z(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f);

		x = x * rot;
		y = y * rot;
		z = z * rot;

		mX = FAMath::Vector3D(x.GetX(), x.GetY(), x.GetZ());
		mY = FAMath::Vector3D(y.GetX(), y.GetY(), y.GetZ());
		mZ = FAMath::Vector3D(z.GetX(), z.GetY(), z.GetZ());

		//orthonormalize the boxs local axes.
		Orthonormalize(mX, mY, mZ);
	}

	void Box::RotateAxes(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Vector4D x(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f);
		FAMath::Vector4D y(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f);
		FAMath::Vector4D z(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f);

		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(rotQuaternion) };
		x = x * rot;
		y = y * rot;
		z = z * rot;

		mX = FAMath::Vector3D(x.GetX(), x.GetY(), x.GetZ());
		mY = FAMath::Vector3D(y.GetX(), y.GetY(), y.GetZ());
		mZ = FAMath::Vector3D(z.GetX(), z.GetY(), z.GetZ());

		//orthonormalize the boxs local axes.
		Orthonormalize(mX, mY, mZ);
	}

	void Box::RotateAxes(float angle, const FAMath::Vector3D axis)
	{
		FAMath::Vector4D x(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f);
		FAMath::Vector4D y(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f);
		FAMath::Vector4D z(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f);

		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(RotationQuaternion(angle, axis)) };

		x = x * rot;
		y = y * rot;
		z = z * rot;

		mX = FAMath::Vector3D(x.GetX(), x.GetY(), x.GetZ());
		mY = FAMath::Vector3D(y.GetX(), y.GetY(), y.GetZ());
		mZ = FAMath::Vector3D(z.GetX(), z.GetY(), z.GetZ());

		//orthonormalize the boxs local axes.
		Orthonormalize(mX, mY, mZ);
	}

	void Box::RotateCenter(const FAMath::Matrix4x4& rot)
	{
		FAMath::Vector4D center(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(), 1.0f);

		center = center * rot;

		mCenter = FAMath::Vector3D(center.GetX(), center.GetY(), center.GetZ());
	}

	void Box::RotateCenter(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Vector4D center(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(), 1.0f);

		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(rotQuaternion) };
		center = center * rot;

		mCenter = FAMath::Vector3D(center.GetX(), center.GetY(), center.GetZ());
	}

	void Box::RotateCenter(float angle, const FAMath::Vector3D axis)
	{
		FAMath::Vector4D center(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(), 1.0f);

		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(RotationQuaternion(angle, axis)) };
		center = center * rot;

		mCenter = FAMath::Vector3D(center.GetX(), center.GetY(), center.GetZ());
	}

	void Box::TranslateCenter(float x, float y, float z)
	{
		mCenter.SetX(mCenter.GetX() + x);
		mCenter.SetY(mCenter.GetY() + y);
		mCenter.SetZ(mCenter.GetZ() + x);
	}

	void Box::TranslateCenter(FAMath::Vector3D v)
	{
		mCenter += v;
	}

	//multiplies the scale, local rotation, and translation in that order.
	//S * LR * T.
	void Box::UpdateLocalToWorldMatrix()
	{
		FAMath::Matrix4x4 localRotation;
		localRotation.SetRow(0, FAMath::Vector4D(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f));
		localRotation.SetRow(1, FAMath::Vector4D(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f));
		localRotation.SetRow(2, FAMath::Vector4D(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mCenter.GetX(), mCenter.GetY(), mCenter.GetZ()) };

		mLocalToWorld = mScale * localRotation * translation;
	}
}