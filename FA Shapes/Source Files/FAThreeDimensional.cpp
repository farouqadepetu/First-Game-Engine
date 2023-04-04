#include "FAThreeDimensional.h"

namespace FAShapes
{
	ThreeDimensionalShapeAbstract::ThreeDimensionalShapeAbstract(const FAColor::Color& color) :
		mCenter{ 0.0f, 0.0f, 0.0f },
		mX{ 1.0f, 0.0f, 0.0f }, mY{ 0.0f, 1.0f, 0.0f }, mZ{ 0.0f, 0.0f, 1.0f }, 
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

	const FAMath::Vector3D& ThreeDimensionalShapeAbstract::GetCenter() const
	{
		return mCenter;
	}


	const FAMath::Vector3D& ThreeDimensionalShapeAbstract::GetXAxis() const
	{
		return mX;
	}

	const FAMath::Vector3D& ThreeDimensionalShapeAbstract::GetYAxis() const
	{
		return mY;
	}

	const FAMath::Vector3D& ThreeDimensionalShapeAbstract::GetZAxis() const
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

	void ThreeDimensionalShapeAbstract::SetCenter(const FAMath::Vector3D& center)
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
		int indexOfFirstVertex, int indexOfConstantData)
	{
		mSphereDrawArguments = { indexCount, locationOfFirstIndex, indexOfFirstVertex, indexOfConstantData };
	}

	void ThreeDimensionalShapeAbstract::RotateAxes(const FAMath::Matrix4x4& rot)
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

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateAxes(const FAMath::Quaternion& rotQuaternion)
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

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateAxes(float angle, const FAMath::Vector3D axis)
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

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateCenter(const FAMath::Matrix4x4& rot)
	{
		FAMath::Vector4D center(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(), 1.0f);

		center = center * rot;

		mCenter = FAMath::Vector3D(center.GetX(), center.GetY(), center.GetZ());

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateCenter(const FAMath::Quaternion& rotQuaternion)
	{
		FAMath::Vector4D center(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(), 1.0f);

		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(rotQuaternion) };
		center = center * rot;

		mCenter = FAMath::Vector3D(center.GetX(), center.GetY(), center.GetZ());

		mUpdateLocalToWorldlMatrix = true;
	}

	void ThreeDimensionalShapeAbstract::RotateCenter(float angle, const FAMath::Vector3D axis)
	{
		FAMath::Vector4D center(mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(), 1.0f);

		FAMath::Matrix4x4 rot{ QuaternionToRotationMatrixRow(RotationQuaternion(angle, axis)) };
		center = center * rot;

		mCenter = FAMath::Vector3D(center.GetX(), center.GetY(), center.GetZ());

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
		mCenter += v;

		mUpdateLocalToWorldlMatrix = true;
	}
}