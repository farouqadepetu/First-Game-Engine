#include "FAThreeDimensional.h"

namespace FAShapes
{
	ThreeDimensionalShapeAbstract::ThreeDimensionalShapeAbstract(const FAColor::Color& color) :
		mCenter{ 0.0f, 0.0f, 0.0f, 1.0f },
		mX{ 1.0f, 0.0f, 0.0f, 0.0f }, mY{ 0.0f, 1.0f, 0.0f, 0.0f }, mZ{ 0.0f, 0.0f, 1.0f, 0.0f },
		mColor{ color },
		mUpdateLocalToWorldlMatrix{ true }
	{}


	/*stores the indices of the vertices that make up each triangle.
	* A quad has two triangles.
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
	void ThreeDimensionalShapeAbstract::Quad(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
	{
		mTriangles.push_back(Triangle(mLocalVertices.data(), a, b, c));
		mTriangles.push_back(Triangle(mLocalVertices.data(), a, c, d));
	}

	void ThreeDimensionalShapeAbstract::CreateNormals()
	{
		//each vertex sums all the normals of the triangles they are a part of.
		for (auto& i : mTriangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			mLocalVertices[i.GetP0Index()].normal += norm;
			mLocalVertices[i.GetP1Index()].normal += norm;
			mLocalVertices[i.GetP2Index()].normal += norm;
		}

		//Normalize the normals
		for (auto& j : mLocalVertices)
		{
			j.normal = Norm(j.normal);
		}
	}

	const FAMath::Vector4D& ThreeDimensionalShapeAbstract::GetCenter() const
	{
		return mCenter;
	}

	const FAMath::Vector4D& ThreeDimensionalShapeAbstract::GetXAxis() const
	{
		return mX;
	}

	const FAMath::Vector4D& ThreeDimensionalShapeAbstract::GetYAxis() const
	{
		return mY;
	}

	const FAMath::Vector4D& ThreeDimensionalShapeAbstract::GetZAxis() const
	{
		return mZ;
	}

	const FAMath::Matrix4x4& ThreeDimensionalShapeAbstract::GetLocalToWorldMatrix() const
	{
		return mLocalToWorld;
	}

	const Vertex* ThreeDimensionalShapeAbstract::GetLocalVertices() const
	{
		return mLocalVertices.data();
	}

	const Triangle* ThreeDimensionalShapeAbstract::GetTriangleList() const
	{
		return mTriangles.data();
	}

	Vertex* ThreeDimensionalShapeAbstract::GetLocalVertices()
	{
		return mLocalVertices.data();
	}

	Triangle* ThreeDimensionalShapeAbstract::GetTriangleList()
	{
		return mTriangles.data();
	}

	const Triangle& ThreeDimensionalShapeAbstract::GetTriangle(unsigned int index) const
	{
		return mTriangles.at(index);
	}

	const DrawArguments& ThreeDimensionalShapeAbstract::GetDrawArguments() const
	{
		return mSphereDrawArguments;
	}

	const FAColor::Color& ThreeDimensionalShapeAbstract::GetColor() const
	{
		return mColor;
	}

	size_t ThreeDimensionalShapeAbstract::GetNumTriangles() const
	{
		return mTriangles.size();
	}

	size_t ThreeDimensionalShapeAbstract::GetNumVertices() const
	{
		return mLocalVertices.size();
	}

	void ThreeDimensionalShapeAbstract::SetCenter(const FAMath::Vector4D& center)
	{
		mCenter = center;
		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::SetCenter(float x, float y, float z)
	{
		mCenter.SetX(x);
		mCenter.SetY(y);
		mCenter.SetZ(z);
		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::SetXAxis(float x, float y, float z)
	{
		mX.SetX(x);
		mX.SetY(y);
		mX.SetZ(z);
		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::SetYAxis(float x, float y, float z)
	{
		mY.SetX(x);
		mY.SetY(y);
		mY.SetZ(z);
		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::SetZAxis(float x, float y, float z)
	{
		mZ.SetX(x);
		mZ.SetY(y);
		mZ.SetZ(z);
		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::SetColor(const FAColor::Color& color)
	{
		mColor = color;

		size_t numVertices = mLocalVertices.size();

		for (size_t i = 0; i < numVertices; ++i)
		{
			mLocalVertices[i].color = mColor;
		}
	}

	void ThreeDimensionalShapeAbstract::SetColor(float r, float g, float b, float a)
	{
		mColor.SetRed(r);
		mColor.SetGreen(g);
		mColor.SetBlue(b);
		mColor.SetAlpha(a);

		unsigned int numVertices = (unsigned int)mLocalVertices.size();

		for (unsigned int i = 0; i < numVertices; ++i)
		{
			mLocalVertices[i].color = mColor;
		}
	}

	void ThreeDimensionalShapeAbstract::SetDrawArguments(const DrawArguments& drawArgs)
	{
		mSphereDrawArguments = drawArgs;
	}

	void ThreeDimensionalShapeAbstract::SetDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex,
		int indexOfFirstVertex, unsigned int indexOfConstantData)
	{
		mSphereDrawArguments = { indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
	}

	void ThreeDimensionalShapeAbstract::RotateAxes(const FAMath::Matrix4x4& rot)
	{
		mX = mX * rot;
		mY = mY * rot;
		mZ = mZ * rot;

		//orthonormalize the boxs local axes.
		Orthonormalize(mX, mY, mZ);

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateAxes(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(rotQuaternion) };

		mX = mX * rot;
		mY = mY * rot;
		mZ = mZ * rot;

		//orthonormalize the boxs local axes.
		Orthonormalize(mX, mY, mZ);

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateAxes(float angle, const FAMath::Vector3D& axis)
	{
		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(RotationQuaternion(angle, axis)) };

		mX = mX * rot;
		mY = mY * rot;
		mZ = mZ * rot;

		//orthonormalize the boxs local axes.
		Orthonormalize(mX, mY, mZ);

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateCenter(const FAMath::Matrix4x4& rot)
	{
		mCenter = mCenter * rot;

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateCenter(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(rotQuaternion) };

		mCenter = mCenter * rot;

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateCenter(float angle, const FAMath::Vector3D& axis)
	{
		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(RotationQuaternion(angle, axis)) };

		mCenter = mCenter * rot;

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::TranslateCenter(float x, float y, float z)
	{
		mCenter.SetX(mCenter.GetX() + x);
		mCenter.SetY(mCenter.GetY() + y);
		mCenter.SetZ(mCenter.GetZ() + x);

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::TranslateCenter(const FAMath::Vector3D& v)
	{
		mCenter += FAMath::Vector4D(v, 0.0f);

		mUpdateLocalToWorldlMatrix = true;
	}
}