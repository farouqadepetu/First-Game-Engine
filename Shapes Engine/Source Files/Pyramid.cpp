#include "Pyramid.h"

namespace ShapesEngine
{
	Pyramid::Pyramid() :
		mWidth{ 1.0f }, mHeight{ 1.0f }, mDepth{ 1.0f }
	{}

	void Pyramid::InitializePyramid(float width, float height, float depth, const vec3 position, const MathEngine::Quaternion orientation,
		const RenderingEngine::Color& color)
	{
		mShape.position = position;
		mShape.orientation = orientation;
		mShape.color = color;

		mWidth = width;
		mHeight = height;
		mDepth = depth;
	}

	const ThreeDimensionalShape& Pyramid::GetShape() const
	{
		return mShape;
	}

	ThreeDimensionalShape& Pyramid::GetShape()
	{
		return mShape;
	}

	float Pyramid::GetWidth() const
	{
		return mWidth;
	}

	float Pyramid::GetHeight() const
	{
		return mHeight;
	}

	float Pyramid::GetDepth() const
	{
		return mDepth;
	}

	void Pyramid::SetWidth(float width)
	{
		mWidth = width;
	}

	void Pyramid::SetHeight(float height)
	{
		mHeight = height;
	}

	void Pyramid::SetDepth(float depth)
	{
		mDepth = depth;
	}

	void Pyramid::UpdateModelMatrix()
	{
		mShape.modelMatrix = MathEngine::Scale4x4(mWidth, mHeight, mDepth) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float Pyramid::Volume()
	{
		return (mWidth * mHeight * mDepth) / 3.0f;
	}
}