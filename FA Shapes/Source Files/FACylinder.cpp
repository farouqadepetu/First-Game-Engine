#include "FACylinder.h"
#include <unordered_map>

namespace FAShapes
{
	Cylinder::Cylinder(float radius, float height, const FAColor::Color& color,
		unsigned int numCircles, unsigned int numVerticesPerCircle) :
		ThreeDimensionalShapeAbstract(color), mRadius{ radius }, mHeight{ height },
		mNumCircles{ numCircles }, mNumVerticesPerCircle{ numVerticesPerCircle }
	{
		if (numCircles < 2)
			mNumCircles = 2;

		if (numVerticesPerCircle < 3)
			mNumVerticesPerCircle = 3;

		CreateVertices();
		CreateTriangles();
		Cylinder::CreateNormals();
	}

	//Creates the vertices for a unit cylinder (r = 1, h = 1) centered around the origin.
	void Cylinder::CreateVertices()
	{
		//Parameteric equations used to produce the vertices of the cylinder
		//x = rsin(θ * u)
		//y = 0.5 - (v * h)
		//z = rcos(θ * u)
		//θ = 360 or 2π
		//r = 1
		//h = 1
		//u = [0, 1]
		//v = [0, 1]
		//y is in the range [0.5, -0.5] so the center of the cylinder is the origin.
		//(u, v) are the texture coordinates for each vertex.

		float u{ 0.0f };
		float v{ 0.0f };
		float uStep{ 1.0f / mNumVerticesPerCircle };
		float vStep{ 1.0f / mNumCircles };
		float h{ 1.0f };

		for (unsigned int i = 0; i <= mNumCircles; ++i)
		{
			for (unsigned int j = 0; j <= mNumVerticesPerCircle; ++j)
			{
				float x{ sin(u * PI2) };
				float y{ 0.5f - (v * h) };
				float z{ cos(u * PI2) };

				//First or last circle
				if (i == 0 || i == mNumCircles)
				{
					mLocalVertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), mColor, FAMath::Vector4D(),
						FAMath::Vector2D((x * 0.5f) + 0.5f, (z * 0.5f) + 0.5f) });
				}
				else
				{
					mLocalVertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(u, v) });
				}

				u += uStep;
			}
			u = 0.0f;
			v += vStep;
		}

		//Center of top circle.
		mLocalVertices.push_back({ FAMath::Vector4D(0.0f, 0.5f, 0.0f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });

		//Center of bottom circle.
		mLocalVertices.push_back({ FAMath::Vector4D(0.0f, -0.5f, 0.0f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });
	}

	void Cylinder::CreateTriangles()
	{
		/* The vertex locations of each vertex in a quad.
		Top Left : (i * n) + j
		Top Right : (i * n) + j + 1
		Bottom Right : ((i + 1) * n) + j + 1
		Bottom Left : ((i + 1) * n) + j
		i is a circle (slice) of the sphere.
		j is a vertex of the ith circle (slice).
		n is the number of vertices per slice.
		*/

		//Make quads with the vertices.
		for (unsigned int i = 0; i < mNumCircles; ++i)
		{
			for (unsigned int j = 0; j < mNumVerticesPerCircle; ++j)
			{
				unsigned int a{ i * (mNumVerticesPerCircle + 1) + j }; //top left

				unsigned int b{ i * (mNumVerticesPerCircle + 1) + j + 1 }; //top right

				unsigned int c{ (i + 1) * (mNumVerticesPerCircle + 1) + j + 1 }; //bottom right

				unsigned int d{ (i + 1) * (mNumVerticesPerCircle + 1) + j }; //bottom left

				Quad(a, d, c, b);
			}
		}

		unsigned int indexCenterVertexTop = (mNumCircles + 1) * (mNumVerticesPerCircle + 1);
		unsigned int indexCenterVertexBottom = indexCenterVertexTop + 1;
		unsigned int indexLastVertexLastCircle = mNumCircles * (mNumVerticesPerCircle + 1);

		//Create triangles with the center of the top circle with the vertices of the top circle.
		//Create triangles with the center of the bottom circle with the vertices of the bottom circle.
		for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			mTriangles.push_back(Triangle(mLocalVertices.data(), indexCenterVertexTop, 
				i, i + 1));

			mTriangles.push_back(Triangle(mLocalVertices.data(), indexCenterVertexBottom,
				indexLastVertexLastCircle + i + 1, indexLastVertexLastCircle + i));
		}
	}

	void Cylinder::CreateNormals()
	{
		//each vertex sums all the normals of the triangles they are a part of.
		for (auto& i : mTriangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			mLocalVertices[i.GetP0Index()].normal += norm;
			mLocalVertices[i.GetP1Index()].normal += norm;
			mLocalVertices[i.GetP2Index()].normal += norm;
		}

		//Sum the normals of the first and last vertex of each circle and give the result to both the first and last vertex.
		for (unsigned int i = 0; i <= mNumCircles; ++i)
		{
			unsigned int firstV = i * (mNumVerticesPerCircle + 1);
			unsigned int lastV = i * (mNumVerticesPerCircle + 1) + mNumVerticesPerCircle;
			auto sumNormal = mLocalVertices[lastV].normal + mLocalVertices[firstV].normal;
			mLocalVertices[lastV].normal = sumNormal;
			mLocalVertices[firstV].normal = sumNormal;
		}

		//Normalize the normals
		for (auto& j : mLocalVertices)
		{
			j.normal = Norm(j.normal);
		}
	}

	float Cylinder::GetRadius() const
	{
		return mRadius;
	}

	float Cylinder::GetHeight() const
	{
		return mHeight;
	}

	void Cylinder::SetRadius(float r)
	{
		mRadius = r;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Cylinder::SetHeight(float h)
	{
		mHeight = h;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Cylinder::UpdateLocalToWorldMatrix()
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

	float Cylinder::Volume()
	{
		return PI * mRadius * mRadius * mHeight;
	}
}