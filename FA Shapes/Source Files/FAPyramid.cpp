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
		mLocalVertices.push_back({ FAMath::Vector3D(0.0f, 0.5f, 0.0f), mColor });

		//base vertices
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, -0.5f, 0.5f), mColor }); //0
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, -0.5f, 0.5f), mColor }); //1
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, -0.5f, -0.5f), mColor }); //2
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, -0.5f, -0.5f), mColor }); //3

	}

	void Pyramid::CreateTriangles()
	{
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 1, 2)); //front triangle
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 3, 4)); //back triangle
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 2, 3)); //right triangle
		mTriangles.push_back(Triangle(mLocalVertices.data(), 0, 4, 1)); //left triangle

		//Base
		Quad(1, 4, 3, 2);

	}

	void Pyramid::CreateNormals()
	{
		//Each vertex has a normal assoicated with it. Each triangle has a normal. A vertex can be a part of multiple triangles.
		//Get the average of all the triangle normals each vertex is a part of.
		//For each vertex we sum all of the triangle normals it is a part of.
		std::unordered_map<unsigned int, std::pair<float, FAMath::Vector3D>> normals;

		size_t numVertices{ mLocalVertices.size() };
		size_t numTriangles{ mTriangles.size() };

		for (int i = 0; i < numTriangles; ++i)
		{
			FAMath::Vector3D norm(mTriangles[i].GetNormal());
			normals[mTriangles[i].GetP0Index()].first += 1.0f;
			normals[mTriangles[i].GetP0Index()].second += norm;

			normals[mTriangles[i].GetP1Index()].first += 1.0f;
			normals[mTriangles[i].GetP1Index()].second += norm;

			normals[mTriangles[i].GetP2Index()].first += 1.0f;
			normals[mTriangles[i].GetP2Index()].second += norm;
		}

		//Take the average to get the normal for that vertex.
		for (int i = 0; i < numVertices; ++i)
		{
			mLocalVertices[i].normal = Norm(normals[i].second / normals[i].first);
		}
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