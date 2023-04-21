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
		ThreeDimensionalShapeAbstract::CreateNormals();

	}

	//Creates all of the vertices for a unit sphere centered around the origin.
	void Sphere::CreateVertices()
	{
		/* Parametric equations of a sphere.
		x = rsinΦcosθ
		y = rcosΦ
		z = -rsinΦsinθ
		theta = [0, 360]
		phi = [0, 180]
		*/

		float phiStep = PI / mNumCircles;
		float thetaStep = PI2 / mNumVerticesPerCircle;
		float phi{ phiStep };
		float theta{ 0.0f };

		std::vector<float> sinPhiValues;
		std::vector<float> cosPhiValues;
		std::vector<float> sinThetaValues;
		std::vector<float> cosThetaValues;

		//Generate all sin(phi) and cos(phi) values between (0, PI) with step rate PI / mNumCircles.
		for (unsigned int i = 0; i < mNumCircles - 1; ++i)
		{
			sinPhiValues.push_back(sin(phi));
			cosPhiValues.push_back(cos(phi));

			phi += phiStep;
		}

		//Generate all sin(theta) and cos(theta) values between [0, 2PI) with step rate 2PI / mNumVerticesPerCircle.
		for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			sinThetaValues.push_back(sin(theta));
			cosThetaValues.push_back(cos(theta));

			theta += thetaStep;
		}

		//Top vertex
		mLocalVertices.push_back({ FAMath::Vector3D(0.0f, 1.0f, 0.0f), mColor, FAMath::Vector3D(0.0f, 1.0f, 0.0f) });

		//Generate the vertices of the sphere using the parametric equations of a sphere.
		for(unsigned int i = 0; i < mNumCircles - 1; ++i)
		{
			for(unsigned int j = 0; j < mNumVerticesPerCircle; ++j)
			{
				float x{ sinPhiValues[i] * cosThetaValues[j]};
				float y{ cosPhiValues[i] };
				float z{ -sinPhiValues[i] * sinThetaValues[j] };

				mLocalVertices.push_back({ FAMath::Vector3D(x, y, z), mColor, FAMath::Vector3D(x, y, z) });
			}
		}

		//Bottom Vertex
		mLocalVertices.push_back({ FAMath::Vector3D(0.0f, -1.0f, 0.0f), mColor, FAMath::Vector3D(0.0f, -1.0f, 0.0f) });
	}

	void Sphere::CreateTriangles()
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

		//make triangles with the top vertex and bottom vertex with the 1st and last circles respectively.
		unsigned int indexLastVertex = (unsigned int)mLocalVertices.size() - 1;
		unsigned int indexFirstVertexLastCircle = (mNumCircles - 1) * (mNumVerticesPerCircle - 1);
		for (unsigned int i = 0; i < mNumVerticesPerCircle; ++i)
		{
			mTriangles.push_back(Triangle(mLocalVertices.data(), 0, i + 1, (i + 1) % mNumVerticesPerCircle + 1));

			mTriangles.push_back(Triangle(mLocalVertices.data(), indexLastVertex,
				(indexFirstVertexLastCircle + i) % mNumVerticesPerCircle + indexFirstVertexLastCircle,
				indexFirstVertexLastCircle + i));
		}

		//Make quads from the vertices not including the first and last vertex.
		for (unsigned int i = 0; i < mNumCircles - 2; ++i)
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