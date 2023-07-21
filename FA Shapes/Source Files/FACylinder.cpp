#include "FACylinder.h"

namespace FAShapes
{
	Cylinder::Cylinder() :
		mRadius{ 1.0f }, mHeight{ 1.0f }
	{}

	void Cylinder::InitializeCylinder(float radius, float height, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
		const FAColor::Color& color)
	{
		mShape.InitializeThreeDimensionalShape(position, orientation, color);

		mRadius = radius;
		mHeight = height;
	}

	const ThreeDimensionalShape& Cylinder::GetShape() const
	{
		return mShape;
	}

	ThreeDimensionalShape& Cylinder::GetShape()
	{
		return mShape;
	}

	float Cylinder::GetRadius() const
	{
		return mRadius;
	}

	float Cylinder::GetHeight() const
	{
		return mHeight;
	}

	void Cylinder::SetRadius(float radius)
	{
		mRadius = radius;
	}

	void Cylinder::SetHeight(float height)
	{
		mHeight = height;
	}

	void Cylinder::UpdateModelMatrix()
	{
		FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mRadius, mHeight, mRadius) };

		FAMath::Matrix4x4 localRotation(FAMath::QuaternionToRotationMatrixRow(mShape.GetOrientation()));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mShape.GetPosition()) };

		mShape.SetModelMatrix(scale * localRotation * translation);
	}

	float Cylinder::Volume()
	{
		return PI * mRadius * mRadius * mHeight;
	}
}