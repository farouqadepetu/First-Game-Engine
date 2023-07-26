#include "Cone.h"

namespace ShapesEngine
{
	Cone::Cone() :
		mRadius{ 1.0f }, mHeight{ 1.0f }
	{}

	void Cone::InitializeCone(float radius, float height, const vec3 position, const MathEngine::Quaternion orientation,
		const RenderingEngine::Color& color)
	{
		mShape.position = position;
		mShape.orientation = orientation;
		mShape.color = color;

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
		mShape.modelMatrix = MathEngine::Scale4x4(mRadius, mHeight, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float Cone::Volume()
	{
		return PI * mRadius * mRadius * (mHeight / 3.0f);
	}
}