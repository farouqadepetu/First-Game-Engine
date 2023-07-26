#include "Sphere.h"

namespace ShapesEngine
{
	Sphere::Sphere() :
		mRadius{ 1.0f }
	{}

	void Sphere::InitializeSphere(float radius, const vec3 position, const MathEngine::Quaternion orientation,
		const RenderingEngine::Color& color)
	{
		mShape.position = position;
		mShape.orientation = orientation;
		mShape.color = color;

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
		mShape.modelMatrix = MathEngine::Scale4x4(mRadius, mRadius, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float Sphere::Volume()
	{
		return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
	}
}