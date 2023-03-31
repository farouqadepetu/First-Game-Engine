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

	//strores the local vertices of box
	void Box::CreateVertices()
	{
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, 0.5f, -0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, 0.5f, -0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, -0.5f, -0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, -0.5f, -0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, 0.5f, 0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, 0.5f, 0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(0.5f, -0.5f, 0.5f), mColor });
		mLocalVertices.push_back({ FAMath::Vector3D(-0.5f, -0.5f, 0.5f), mColor });
	}

	//creates the triangles from the local vertices
	void Box::CreateTriangles()
	{
		//the indices of the vertices that make up each face of the box
		Quad(0, 1, 2, 3); //back face
		Quad(5, 4, 7, 6); //front face
		Quad(4, 5, 1, 0); //top face
		Quad(3, 2, 6, 7); //bottom face
		Quad(4, 0, 3, 7); //left face
		Quad(1, 5, 6, 2); //right face
	}

	//creates the normals from the triangles
	void Box::CreateNormals()
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
		mUpdateModelMatrix = true;
	}

	void Box::SetHeight(float height)
	{
		mHeight = height;
		mUpdateModelMatrix = true;
	}

	void Box::SetDepth(float depth)
	{
		mDepth = depth;
		mUpdateModelMatrix = true;
	}

	void Box::UpdateLocalToWorldMatrix()
	{
		if (mUpdateModelMatrix)
		{
			FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mWidth, mHeight, mDepth) };

			FAMath::Matrix4x4 localRotation;
			localRotation.SetRow(0, FAMath::Vector4D(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f));
			localRotation.SetRow(1, FAMath::Vector4D(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f));
			localRotation.SetRow(2, FAMath::Vector4D(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f));

			FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mCenter.GetX(), mCenter.GetY(), mCenter.GetZ()) };

			mLocalToWorld = scale * localRotation * translation;

			mUpdateModelMatrix = false;
		}
	}
}