#include "FASphere.h"

namespace FAShapes
{
	Sphere::Sphere(float radius, const FAColor::Color& color, unsigned int numCircles, unsigned int numVerticesPerCircle) :
		ThreeDimensionalShapeAbstract(color),
		mRadius{ radius }, mNumCircles{ numCircles }, mNumVerticesPerCircle{ numVerticesPerCircle }
	{
		if (numCircles < 2)
			mNumCircles = 2;

		if (numVerticesPerCircle < 3)
			mNumVerticesPerCircle = 3;

		CreateVertices();
		CreateTriangles();
		Sphere::CreateNormals();
	}

	void Sphere::CreateVertices()
	{
		/* Parametric equations of a sphere.
		x = rsin(Φ * v)cos(θ * u)
		y = rcos(Φ * v)
		z = rsin(Φ * v)sin(θ * u)
		θ = 360 or 2π
		Φ = 180 or π
		r = 1
		u = [0, 1]
		v = [0, 1]
		(u, v) are the texture coordinates for each vertex.
		*/

		//(u, v) for the texture coordinates for each vertex
		float u{ 0.0f };
		float v{ 0.0f };
		float uStep{ 1.0f / mNumVerticesPerCircle };
		float vStep{ 1.0f / mNumCircles };

		//Generate the vertices of the sphere
		for (int i = 0; i <= mNumCircles; ++i)
		{
			for (int j = 0; j <= mNumVerticesPerCircle; ++j)
			{
				float x{ sin(v * PI) * sin(u * PI2) };
				float y{ cos(v * PI) };
				float z{ sin(v * PI) * cos(u * PI2) };

				mLocalVertices.push_back({ FAMath::Vector4D(x, y, z, 1.0f), mColor, FAMath::Vector4D(x, y, z, 0.0f), FAMath::Vector2D(u, v) });

				u += uStep;
			}
			v += vStep;
			u = 0.0f;
		}
	}

	void Sphere::CreateTriangles()
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
	}

	void Sphere::CreateNormals()
	{
		size_t numVertices{ mLocalVertices.size() };

		for (size_t i = 0; i < numVertices; ++i)
		{
			//Make each normal a unit vector.
			mLocalVertices[i].normal = Norm(mLocalVertices[i].normal);
		}
	}

	float Sphere::GetRadius() const
	{
		return mRadius;
	}

	void Sphere::SetRadius(float r)
	{
		mRadius = r;
		mUpdateLocalToWorldlMatrix = true;
	}

	void Sphere::UpdateLocalToWorldMatrix()
	{
		if (mUpdateLocalToWorldlMatrix)
		{
			FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mRadius, mRadius, mRadius) };

			FAMath::Matrix4x4 localRotation;
			localRotation.SetRow(0, FAMath::Vector4D(mX.GetX(), mX.GetY(), mX.GetZ(), 0.0f));
			localRotation.SetRow(1, FAMath::Vector4D(mY.GetX(), mY.GetY(), mY.GetZ(), 0.0f));
			localRotation.SetRow(2, FAMath::Vector4D(mZ.GetX(), mZ.GetY(), mZ.GetZ(), 0.0f));

			FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mCenter.GetX(), mCenter.GetY(), mCenter.GetZ()) };

			mLocalToWorld = scale * localRotation * translation;

			mUpdateLocalToWorldlMatrix = false;
		}
	}

	float Sphere::Volume()
	{
		return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
	}

}