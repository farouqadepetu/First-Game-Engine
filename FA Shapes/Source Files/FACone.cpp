#include "FACone.h"
#include <unordered_map>

namespace FAShapes
{
	Cone::Cone(float radius, float height,  const FAColor::Color& color, bool fillBottom,
		unsigned int numCircles, unsigned int numVerticesPerCircle) :
		ThreeDimensionalShapeAbstract(color), mRadius{ radius }, mHeight{ height }, mFillBottom{ fillBottom },
		mNumCircles{ numCircles }, mNumVerticesPerCircle{ numVerticesPerCircle }
	{
		if (numCircles < 2)
			mNumCircles = 2;

		if (numVerticesPerCircle < 3)
			mNumVerticesPerCircle = 3;

		CreateVertices();
		CreateTriangles();
		CreateNormals();
	}

	//Creates a unit cone (max radius = 1.0f) where the cone grows from y = 1 to the x-z plane.
	void Cone::CreateVertices()
	{
		//Parameteric equations of a cone.
		//x = rsinθ
		//y = r
		//z = rcosθ
		//θ = [0, 360]
		//r = [0, max radius]

		float radius{ 0.0f };
		float radiusStep{ 1.0f / mNumCircles };
		float theta{ 0.0f };
		float thetaStep{ PI2 / mNumVerticesPerCircle };

		std::vector<float> sinValues;
		std::vector<float> cosValues;

		for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			sinValues.push_back(sin(theta));
			cosValues.push_back(cos(theta));

			theta += thetaStep;
		}

		//Top vertex
		mLocalVertices.push_back({ FAMath::Vector3D(0.0f, 0.5f, 0.0f), mColor });

		radius += radiusStep;
		for (unsigned int i = 0; i < mNumCircles; ++i)
		{
			for (unsigned int j = 0; j < mNumVerticesPerCircle; ++j)
			{
				float x{ radius * sinValues[j] };
				float y{ 0.5f - radius };
				float z{ radius * cosValues[j] };

				mLocalVertices.push_back({ FAMath::Vector3D(x, y, z), mColor });
			}
			radius += radiusStep;
		}

		if (mFillBottom)
		{
			//add the center of the bottom cirlce.
			mLocalVertices.push_back({ FAMath::Vector3D(0.0f, -0.5f, 0.0f), mColor });
		}
	}

	void Cone::CreateTriangles()
	{
		/* The vertex locations of each vertex in a quad.
		Top Left : (i * n) + j + 2
		Top Right : (i * n) + j + 1
		Bottom Right : ((i + 1) * n) + j + 1
		Bottom Left : ((i + 1) * n) + j + 2
		i is a circle (slice) of the sphere.
		j is a vertex of the ith circle (slice).
		n is the number of vertices per slice.
		*/

		//Create triangles with the top vertex and the vertices of the first circle.
		for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			mTriangles.push_back(Triangle(mLocalVertices.data(), 0, i + 1, (i + 1) % mNumVerticesPerCircle + 1));
		}

		//Make quads wih the vertices the circles.
		for (unsigned int i = 0; i < mNumCircles - 1; ++i)
		{
			unsigned int indexFirstVertexFirstCircle{ i * mNumVerticesPerCircle + 1 };
			unsigned int indexFirstVertexSecondCricle{ (i + 1) * mNumVerticesPerCircle + 1 };

			for (unsigned int j = 0; j < mNumVerticesPerCircle; ++j)
			{
				unsigned int a{ (i * mNumVerticesPerCircle + j + 1) % mNumVerticesPerCircle + indexFirstVertexFirstCircle }; //top left

				unsigned int b{ i * mNumVerticesPerCircle + j + 1 }; //top right

				unsigned int c{ (i + 1) * mNumVerticesPerCircle + j + 1 }; //bottom right

				unsigned int d{ ((i + 1) * mNumVerticesPerCircle + j + 1) % mNumVerticesPerCircle +
					indexFirstVertexSecondCricle }; //bottom left

				Quad(a, b, c, d);
			}
		}

		if (mFillBottom)
		{
			//Make triangles with the center of the bottom cirlce with the vertices of the bottom circle.

			unsigned int indexLastVertex = mLocalVertices.size() - 1;
			unsigned int indexFirstVertexLastCircle = (mNumCircles - 1) * mNumVerticesPerCircle;
			for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
			{
				mTriangles.push_back(Triangle(mLocalVertices.data(), indexLastVertex,
					(indexFirstVertexLastCircle + i + 1) % mNumVerticesPerCircle + indexFirstVertexLastCircle + 1,
					indexFirstVertexLastCircle + i + 1));
			}
		}
	}

	void Cone::CreateNormals()
	{
		//Each vertex has a normal assoicated with it. Each triangle has a normal. A vertex can be a part of multiple triangles.
		//Get the average of all the triangle normals each vertex is a part of.
		//For each vertex we sum all of the triangle normals it is a part of.
		std::unordered_map<unsigned int, std::pair<float, FAMath::Vector3D>> normals;

		size_t numVertices{ mLocalVertices.size() };
		size_t numTriangles{ mTriangles.size() };

		for (size_t i = 0; i < numTriangles; ++i)
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
		for (size_t i = 0; i < numVertices; ++i)
		{
			mLocalVertices[i].normal = Norm(normals[i].second / normals[i].first);
		}
	}

	float Cone::GetRadius() const
	{
		return mRadius;
	}

	float Cone::GetHeight() const
	{
		return mHeight;
	}

	void Cone::SetRadius(float r)
	{
		mRadius = r;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Cone::SetHeight(float h)
	{
		mHeight = h;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Cone::UpdateLocalToWorldMatrix()
	{
		if (mUpdateLocalToWorldlMatrix)
		{
			FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mRadius, mHeight, mRadius) };

			FAMath::Matrix4x4 localRotation;
			localRotation.SetRow(0, FAMath::Vector4D(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f));
			localRotation.SetRow(1, FAMath::Vector4D(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f));
			localRotation.SetRow(2, FAMath::Vector4D(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f));

			FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mCenter.GetX(), mCenter.GetY(), mCenter.GetZ()) };

			mLocalToWorld = scale * localRotation * translation;

			mUpdateLocalToWorldlMatrix = false;
		}
	}


	float Cone::Volume()
	{
		return PI * mRadius * mRadius * (mHeight / 3.0f);
	}
}