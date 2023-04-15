#include "FABox.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace FAShapes
{
	Box::Box(float width, float height, float depth, const FAColor::Color& color) :
		ThreeDimensionalShapeAbstract(color), mWidth{ width }, mHeight{ height }, mDepth{ depth }
	{
		CreateVertices();
		CreateTriangles();
		CreateNormals();
	}

	//strores the local vertices of the box
	void Box::CreateVertices()
	{
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, 0.5f, 0.5f), mColor });		//0
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, 0.5f, 0.5f), mColor });		//1
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, -0.5f, 0.5f), mColor });		//2
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, -0.5f, 0.5f), mColor });		//3
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, 0.5f, -0.5f), mColor });		//4
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, 0.5f, -0.5f), mColor });		//5
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, -0.5f, -0.5f), mColor });		//6
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, -0.5f, -0.5f), mColor });	//7
	}

	//creates the triangles from the local vertices
	void Box::CreateTriangles()
	{
		//the indices of the vertices that make up each face of the box
		Quad(1, 0, 3, 2); //front face
		Quad(4, 5, 6, 7); //back face
		Quad(0, 1, 5, 4); //top face
		Quad(7, 6, 2, 3); //bottom face
		Quad(0, 4, 7, 3); //left face
		Quad(5, 1, 2, 6); //right face
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
		mUpdateLocalToWorldlMatrix = true;
	}

	void Box::SetHeight(float height)
	{
		mHeight = height;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Box::SetDepth(float depth)
	{
		mDepth = depth;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Box::UpdateLocalToWorldMatrix()
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

	float Box::Volume()
	{
		return mWidth * mHeight * mDepth;
	}
}