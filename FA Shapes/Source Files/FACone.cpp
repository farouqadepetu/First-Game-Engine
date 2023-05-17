#include "FACone.h"
#include <unordered_map>

namespace FAShapes
{
	Cone::Cone(float radius, float height,  const FAColor::Color& color,
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
		Cone::CreateNormals();
	}

	//Creates a unit cone (max radius = 1.0f) where the cone grows from y = 1 to the x-z plane.
	void Cone::CreateVertices()
	{
		//Parameteric equations used to the produce the vertices of a cone.
		//x = (v * r) * sin(θ * u)
		//y = 0.5 - (v * r)
		//z = (v * r) * cos(θ * u)
		//radius = 1
		//θ = 360 or 2π
		//u = [0, 1]
		//v = [0, 1]
		//(u, v) are the texture coordinates for each vertex.
		//y is in the range [0.5, -0.5] so the center of the cone is the origin.

		float radius{ 1.0f };
		float u{ 0.0f };
		float v{ 0.0f };
		float uStep{ 1.0f / mNumVerticesPerCircle };
		float vStep{ 1.0f / mNumCircles };

		//Generate the vertices of the cone
		for (int i = 0; i <= mNumCircles; ++i)
		{
			for (int j = 0; j <= mNumVerticesPerCircle; ++j)
			{
				float x{ (v * radius) * sin(u * PI2) };
				float y{ 0.5f - (v * radius) };
				float z{ (v * radius) * cos(u * PI2) };

				//Last circle
				if (i == mNumCircles)
				{
					mLocalVertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), mColor, FAMath::Vector4D(), 
						FAMath::Vector2D((x * 0.5f) + 0.5f, (z * 0.5f) + 0.5f) });
				}
				else //all other circles
				{
					mLocalVertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(u, v) });
				}

				u += uStep;
			}
			v += vStep;
			u = 0.0f;
		}

		//Center of bottom circle.
		mLocalVertices.push_back({ FAMath::Vector4D(0.0f, -0.5f, 0.0f, 1.0f), mColor, FAMath::Vector4D(), FAMath::Vector2D(0.5f, 0.5f) });
	}

	void Cone::CreateTriangles()
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

		unsigned int indexCenterVertexBottom = (mNumCircles + 1) * (mNumVerticesPerCircle + 1);
		unsigned int indexLastVertexLastCircle = mNumCircles * (mNumVerticesPerCircle + 1);

		//Create triangles with the center of the bottom circle with the vertices of the bottom circle.
		for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			mTriangles.push_back(Triangle(mLocalVertices.data(), indexCenterVertexBottom,
				indexLastVertexLastCircle + i + 1, indexLastVertexLastCircle + i));
		}
	}

	void Cone::CreateNormals()
	{
		//each vertex sums all the normals of the triangles they are a part of.
		for (auto& i : mTriangles)
		{
			FAMath::Vector4D norm(i.GetNormal());
			mLocalVertices[i.GetP0Index()].normal += norm;
			mLocalVertices[i.GetP1Index()].normal += norm;
			mLocalVertices[i.GetP2Index()].normal += norm;
		}

		//Sum the normals of the first and last vertex of each circle and give the result to both.
		//Sum the normals of the vertices of the first circle and give the result to each vertex.
		//The vertices of the first circle have the same vertex position but have different texture coordinates. They all need to have the 
		//same normal for lighting.
		//The same thing for the first and last vertex for each circle.
		for (unsigned int i = 0; i <= mNumCircles; ++i)
		{
			if (i == 0)
			{
				FAMath::Vector4D normalSum;
				for (unsigned int j = 0; j <= mNumVerticesPerCircle; ++j)
				{
					normalSum += mLocalVertices[j].normal;
				}

				for (unsigned int j = 0; j <= mNumVerticesPerCircle; ++j)
				{
					 mLocalVertices[j].normal = normalSum;
				}
			}
			else
			{
				unsigned int firstV = i * (mNumVerticesPerCircle + 1);
				unsigned int lastV = i * (mNumVerticesPerCircle + 1) + mNumVerticesPerCircle;
				auto sumNormal = mLocalVertices[lastV].normal + mLocalVertices[firstV].normal;
				mLocalVertices[lastV].normal = sumNormal;
				mLocalVertices[firstV].normal = sumNormal;
			}
		}

		//Sum the normal of the center vertex of the bottom circle with each vertex and give the sum to the vertex.
		//This will make the normal point more downward, 
		//which means there will be better interpolation from the center to the edges of the circle.
		unsigned int indexFirstVertexLastCircle = mNumCircles * (mNumVerticesPerCircle + 1);
		unsigned int indexCenterVertexBottomCircle = (mNumCircles + 1) * (mNumVerticesPerCircle + 1);
		for (unsigned int i = 0; i <= mNumVerticesPerCircle; ++i)
		{
			auto sumNormal = (mLocalVertices[indexCenterVertexBottomCircle].normal * 0.25f) + mLocalVertices[indexFirstVertexLastCircle + i].normal;
			mLocalVertices[indexFirstVertexLastCircle + i].normal = sumNormal;
		}

		//Normalize the normals
		for (auto& j : mLocalVertices)
		{
			j.normal = Norm(j.normal);
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