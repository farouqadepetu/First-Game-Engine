#include "FACone.h"

namespace FAShapes
{
	Cone::Cone() :
		mRadius{ 1.0f }, mHeight{ 1.0f }
	{}

	void Cone::InitializeCone(float radius, float height, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
		const FAColor::Color& color)
	{
		mShape.InitializeThreeDimensionalShape(position, orientation, color);

		mRadius = radius;
		mHeight = height;
	}

	const ThreeDimensionalShape& Cone::GetShape() const
	{
		return mShape;
	}

	ThreeDimensionalShape& Cone::GetShape()
	{
		return mShape;
	}

	float Cone::GetRadius() const
	{
		return mRadius;
	}

	float Cone::GetHeight() const
	{
		return mHeight;
	}

	void Cone::SetRadius(float radius)
	{
		mRadius = radius;
	}

	void Cone::SetHeight(float height)
	{
		mHeight = height;
	}

	void Cone::UpdateModelMatrix()
	{
		FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mRadius, mHeight, mRadius) };

		FAMath::Matrix4x4 localRotation(FAMath::QuaternionToRotationMatrixRow(mShape.GetOrientation()));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mShape.GetPosition()) };

		mShape.SetModelMatrix(scale * localRotation * translation);
	}

	float Cone::Volume()
	{
		return PI * mRadius * mRadius * (mHeight / 3.0f);
	}
}