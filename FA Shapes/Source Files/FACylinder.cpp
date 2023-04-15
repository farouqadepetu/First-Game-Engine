#include "FACylinder.h"
#include <unordered_map>

namespace FAShapes
{
	Cylinder::Cylinder(float radius, float height, const FAColor::Color& color,
		bool fillTopBottom,
		unsigned int numCircles, unsigned int numVerticesPerCircle) :
		ThreeDimensionalShapeAbstract(color), mRadius{ radius }, mHeight{ height },
		mFillTopAndBottom{ fillTopBottom },
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

	//Creates the vertices for a unit cylinder (r = 1, h = 1) centered around the origin.
	void Cylinder::CreateVertices()
	{
		//Parameteric equations of a cylinder
		//x = rsinθ
		//y = h
		//z = rcosθ
		//θ = [0, 360]
		//h = [-h / 2, h / 2]

		float theta{ 0.0f };
		float thetaStep{ PI2 / mNumVerticesPerCircle };
		float height{ 0.5f };
		float heightStep{ 1.0f / mNumCircles };

		std::vector<float> cosValues;
		std::vector<float> sinValues;

		//Generate all the cos and sin values between 0 - 2PI with the step rate being 2 * PI / mNumVerticesPerCircle.
		for(unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			cosValues.push_back(cos(theta));
			sinValues.push_back(sin(theta));

			theta += thetaStep;
		}

		//Generate the vertices of the cylinder.
		for (unsigned int i = 0; i <= mNumCircles; ++i)
		{
			for(unsigned int j = 0; j < mNumVerticesPerCircle; ++j)
			{
				float x{ sinValues[j] };
				float y{ height };
				float z{ cosValues[j] };

				mLocalVertices.push_back({ FAMath::Vector3D(x, y, z), mColor });
			}
			height -= heightStep;
		}

		if (mFillTopAndBottom)
		{
			//Add center of the top and bottom circles.

			//Center of top circle.
			mLocalVertices.push_back({ FAMath::Vector3D(0.0f, 0.5f, 0.0f), mColor });

			//Center of bottom circle.
			mLocalVertices.push_back({ FAMath::Vector3D(0.0f, -0.5f, 0.0f), mColor });
		}
	}

	void Cylinder::CreateTriangles()
	{
		/* The vertex locations of each vertex in a quad.
		Top Left : (i * n) + j + 1
		Top Right : (i * n) + j
		Bottom Right : ((i + 1) * n) + j
		Bottom Left : ((i + 1) * n) + j + 1
		i is a circle (slice) of the sphere.
		j is a vertex of the ith circle (slice).
		n is the number of vertices per slice.
		*/

		//Make quads with the vertices.
		for (unsigned int i = 0; i < mNumCircles; ++i)
		{
			unsigned int indexFirstVertexFirstCircle{ i * mNumVerticesPerCircle };
			unsigned int indexFirstVertexSecondCricle{ (i + 1) * mNumVerticesPerCircle };

			for (unsigned int j = 0; j < mNumVerticesPerCircle; ++j)
			{
				unsigned int a{ (i * mNumVerticesPerCircle + j + 1) % mNumVerticesPerCircle + indexFirstVertexFirstCircle }; //top left

				unsigned int b{ i * mNumVerticesPerCircle + j }; //top right

				unsigned int c{ (i + 1) * mNumVerticesPerCircle + j }; //bottom right

				unsigned int d{ ((i + 1) * mNumVerticesPerCircle + j + 1) % mNumVerticesPerCircle + 
					indexFirstVertexSecondCricle }; //bottom left

				Quad(a, b, c, d);
			}
		}

		if (mFillTopAndBottom)
		{
			unsigned int indexCenterTopVertex = mLocalVertices.size() - 2;
			unsigned int indexCenterBottomVertex = mLocalVertices.size() - 1;
			unsigned int indexFirstVertexLastCircle = mNumCircles * mNumVerticesPerCircle;

			//Create triangles with the center of the top circle with the vertices of the top circle.
			for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
			{
				mTriangles.push_back(Triangle(mLocalVertices.data(), indexCenterTopVertex, i, (i + 1) % mNumVerticesPerCircle));
			}

			//Create triangles with the center of the bottom circle with the vertices of the bottom circle.
			for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
			{
				mTriangles.push_back(Triangle(mLocalVertices.data(), indexCenterBottomVertex, 
					(indexFirstVertexLastCircle + i + 1) % mNumVerticesPerCircle + indexFirstVertexLastCircle,
					indexFirstVertexLastCircle + i));
			}
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