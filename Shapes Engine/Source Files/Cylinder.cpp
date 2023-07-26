#include "Cylinder.h"

namespace ShapesEngine
{
	Cylinder::Cylinder() :
		mRadius{ 1.0f }, mHeight{ 1.0f }
	{}

	void Cylinder::InitializeCylinder(float radius, float height, const vec3 position, const MathEngine::Quaternion orientation,
		const RenderingEngine::Color& color)
	{
		mShape.position = position;
		mShape.orientation = orientation;
		mShape.color = color;

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
		mShape.modelMatrix = MathEngine::Scale4x4(mRadius, mHeight, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float Cylinder::Volume()
	{
		return PI * mRadius * mRadius * mHeight;
	}
}