#include "RigidSphere.h"

namespace PhysicsEngine
{
	//-----------------------------------------------------------------------------------------------------------------------------
	//RigidSphere Properties

	RigidSphere::RigidSphere() : mRadius{ 1.0f }
	{}

	void RigidSphere::InitializeRigidSphere(float radius, float massDensity,
		const vec3& initialPosition, const MathEngine::Quaternion& initialOrientation, const RenderingEngine::Color& color,
		const std::vector<ShapesEngine::Vertex>& vertices, const std::vector<ShapesEngine::Triangle>& triangles)
	{
		mRadius = radius;

		mShape.position = initialPosition;
		mShape.orientation = initialOrientation;
		mShape.color = color;

		//Need to scale the vertices to get it to the proper size so the computed mass is dependent on the volume of the object.
		std::vector<ShapesEngine::Vertex> tempVertexList = vertices;
		std::vector<ShapesEngine::Triangle> tempTriangleList = triangles;
		mat3 scale(MathEngine::Scale(radius, radius, radius));

		for (auto& i : tempVertexList)
		{
			i.position = i.position * scale;
		}

		//Set the vertex list the triangles in the temp triangle vector point to to the temp vertex list.
		for (auto& i : tempTriangleList)
		{
			i.vertexList = tempVertexList.data();
		}

		mRigidBody.InitializeRigidBody(massDensity, initialOrientation, tempTriangleList);

		//Formula used is:
		//local origin world location = center of mass world locaiton - offset at t = 0
		//so offset = center of mass world locaiton - local origin world location at t = 0.
		//At t = 0 the local origin world location is (0, 0, 0) so offset = center of mass world locaiton.
		mOffset = mRigidBody.GetCenterOfMass();

		//Move the center of mass to its initial location in the world.
		mRigidBody.SetCenterOfMass(mShape.position + mOffset);
	}

	float RigidSphere::GetRadius() const
	{
		return mRadius;
	}
	void RigidSphere::SetRadius(float radius)
	{
		mRadius = radius;
	}

	const RigidBody& RigidSphere::GetRigidBody() const
	{
		return mRigidBody;
	}

	RigidBody& RigidSphere::GetRigidBody()
	{
		return mRigidBody;
	}

	const ShapesEngine::ThreeDimensionalShape& RigidSphere::GetShape() const
	{
		return mShape;
	}

	ShapesEngine::ThreeDimensionalShape& RigidSphere::GetShape()
	{
		return mShape;
	}

	void RigidSphere::SetPosition(const vec3& position)
	{
		mShape.position = position;
		mRigidBody.SetCenterOfMass(position + mOffset);
	}

	void RigidSphere::UpdateModelMatrix()
	{
		mShape.position = mRigidBody.GetCenterOfMass() - mOffset;
		mShape.orientation = mRigidBody.GetOrientation();

		mShape.modelMatrix = MathEngine::Scale4x4(mRadius, mRadius, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float RigidSphere::Volume()
	{
		return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
	}
}