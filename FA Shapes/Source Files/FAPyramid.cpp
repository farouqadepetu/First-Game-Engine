#include "FAPyramid.h"
#include <unordered_map>

namespace FAShapes
{
	Pyramid::Pyramid(float width, float height, float depth,
		const FAColor::Color& color) :
		ThreeDimensionalShapeAbstract(color), mWidth{ width }, mHeight{ height }, mDepth{ depth }
	{
		CreateVertices();
		CreateTriangles();
		CreateNormals();
	}

	void Pyramid::CreateVertices()
	{
		//top vertex
		mLocalVertices.push_back({ FAMath::Vector4D(0.0f, 0.5f, 0.0f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });

		//base vertices
		mLocalVertices.push_back({ FAMath::Vector4D(-0.5f, -0.5f, 0.5f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(0.0f, 0.0f) }); //1
		mLocalVertices.push_back({ FAMath::Vector4D(0.5f, -0.5f, 0.5f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(1.0f, 0.0f) }); //2
		mLocalVertices.push_back({ FAMath::Vector4D(0.5f, -0.5f, -0.5f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(1.0f, 1.0f) }); //3
		mLocalVertices.push_back({ FAMath::Vector4D(-0.5f, -0.5f, -0.5f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(0.0f, 1.0f) }); //4

	}

	void Pyramid::CreateTriangles()
	{
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 1, 2)); //front triangle
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 3, 4)); //back triangle
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 2, 3)); //right triangle
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 4, 1)); //left triangle

		//Base
		Quad(4, 3, 2, 1);

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
		mUpdateLocalToWorldlMatrix = true;
	}

	void Pyramid::SetHeight(float height)
	{
		mHeight = height;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Pyramid::SetDepth(float depth)
	{
		mDepth = depth;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Pyramid::UpdateLocalToWorldMatrix()
	{
		if (mUpdateLocalToWorldlMatrix)
		{
			FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mWidth, mHeight, mDepth) };

			FAMath::Matrix4x4 localRotation;
			localRotation.SetRow(0, FAMath::Vector4D(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f));
			localRotation.SetRow(1, FAMath::Vector4D(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f));
			localRotation.SetRow(2, FAMath::Vector4D(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f));

			FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mCenter.GetX(), mCenter.GetY(), mCenter.GetZ()) };

			mLocalToWorld = scale * localRotation * translation;

			mUpdateLocalToWorldlMatrix = false;
		}
	}

	float Pyramid::Volume()
	{
		return (mWidth * mHeight * mDepth) / 3.0f;
	}
}