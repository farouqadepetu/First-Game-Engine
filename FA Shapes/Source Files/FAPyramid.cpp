#include "FAPyramid.h"

namespace FAShapes
{
	Pyramid::Pyramid() :
		mWidth{ 1.0f }, mHeight{ 1.0f }, mDepth{ 1.0f }
	{}

	void Pyramid::InitializePyramid(float width, float height, float depth, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
		const FAColor::Color& color)
	{
		mShape.InitializeThreeDimensionalShape(position, orientation, color);

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
		FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mWidth, mHeight, mDepth) };

		FAMath::Matrix4x4 localRotation(FAMath::QuaternionToRotationMatrixRow(mShape.GetOrientation()));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mShape.GetPosition()) };

		mShape.SetModelMatrix(scale * localRotation * translation);
	}

	float Pyramid::Volume()
	{
		return (mWidth * mHeight * mDepth) / 3.0f;
	}
}