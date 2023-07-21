#include "FASphere.h"

namespace FAShapes
{
	Sphere::Sphere() :
		mRadius{ 1.0f }
	{}

	void Sphere::InitializeSphere(float radius, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
		const FAColor::Color& color)
	{
		mShape.InitializeThreeDimensionalShape(position, orientation, color);

		mRadius = radius;
	}

	const ThreeDimensionalShape& Sphere::GetShape() const
	{
		return mShape;
	}

	ThreeDimensionalShape& Sphere::GetShape()
	{
		return mShape;
	}

	float Sphere::GetRadius() const
	{
		return mRadius;
	}

	void Sphere::SetRadius(float radius)
	{
		mRadius = radius;
	}

	void Sphere::UpdateModelMatrix()
	{
		FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mRadius, mRadius, mRadius) };

		FAMath::Matrix4x4 localRotation(FAMath::QuaternionToRotationMatrixRow(mShape.GetOrientation()));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mShape.GetPosition()) };

		mShape.SetModelMatrix(scale * localRotation * translation);
	}

	float Sphere::Volume()
	{
		return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
	}
}