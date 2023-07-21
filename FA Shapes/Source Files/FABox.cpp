#include "FABox.h"

namespace FAShapes
{
	Box::Box() :
		mWidth{ 1.0f }, mHeight{ 1.0f }, mDepth{ 1.0f }
	{}

	void Box::InitializeBox(float width, float height, float depth, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
		const FAColor::Color& color)
	{
		mShape.InitializeThreeDimensionalShape(position, orientation, color);

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
		FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mWidth, mHeight, mDepth) };

		FAMath::Matrix4x4 localRotation(FAMath::QuaternionToRotationMatrixRow(mShape.GetOrientation()));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mShape.GetPosition()) };

		mShape.SetModelMatrix(scale * localRotation * translation);
	}

	float Box::Volume()
	{
		return mWidth * mHeight * mDepth;
	}
}