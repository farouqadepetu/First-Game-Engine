#include "Box.h"

namespace ShapesEngine
{
	Box::Box() :
		mWidth{ 1.0f }, mHeight{ 1.0f }, mDepth{ 1.0f }
	{}

	void Box::InitializeBox(float width, float height, float depth, const vec3 position, const MathEngine::Quaternion orientation,
		const RenderingEngine::Color& color)
	{
		mShape.position = position;
		mShape.orientation = orientation;
		mShape.color = color;

		mWidth = width;
		mHeight = height;
		mDepth = depth;
	}

	const ThreeDimensionalShape& Box::GetShape() const
	{
		return mShape;
	}

	ThreeDimensionalShape& Box::GetShape()
	{
		return mShape;
	}

	float Box::GetWidth() const
	{
		return mWidth;
	}

	float Box::GetHeight() const
	{
		return mHeight;
	}

	float Box::GetDepth() const
	{
		return mDepth;
	}

	void Box::SetWidth(float width)
	{
		mWidth = width;
	}

	void Box::SetHeight(float height)
	{
		mHeight = height;
	}

	void Box::SetDepth(float depth)
	{
		mDepth = depth;
	}

	void Box::UpdateModelMatrix()
	{
		mShape.modelMatrix = MathEngine::Scale4x4(mWidth, mHeight, mDepth) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float Box::Volume()
	{
		return mWidth * mHeight * mDepth;
	}
}