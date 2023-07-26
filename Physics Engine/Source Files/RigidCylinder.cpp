#include "RigidCylinder.h"

namespace PhysicsEngine
{
	//-----------------------------------------------------------------------------------------------------------------------------
	//RigidCylinder Properties

	RigidCylinder::RigidCylinder() : mRadius{ 1.0f }, mHeight{ 1.0f }
	{}

	void RigidCylinder::InitializeRigidCylinder(float radius, float height, float massDensity,
		const vec3& initialPosition, const MathEngine::Quaternion& initialOrientation, const RenderingEngine::Color& color,
		const std::vector<ShapesEngine::Vertex>& vertices, const std::vector<ShapesEngine::Triangle>& triangles)
	{
		mRadius = radius;
		mHeight = height;

		mShape.position = initialPosition;
		mShape.orientation = initialOrientation;
		mShape.color = color;

		//Need to scale the vertices to get it to the proper size so the computed mass is dependent on the volume of the object.
		std::vector<ShapesEngine::Vertex> tempVertexList = vertices;
		std::vector<ShapesEngine::Triangle> tempTriangleList = triangles;
		MathEngine::Matrix3x3 scale(MathEngine::Scale(radius, height, radius));

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

	float RigidCylinder::GetRadius() const
	{
		return mRadius;
	}

	float RigidCylinder::GetHeight() const
	{
		return mHeight;
	}

	void RigidCylinder::SetRadius(float radius)
	{
		mRadius = radius;
	}

	void RigidCylinder::SetHeight(float height)
	{
		mHeight = height;
	}

	const RigidBody& RigidCylinder::GetRigidBody() const
	{
		return mRigidBody;
	}

	RigidBody& RigidCylinder::GetRigidBody()
	{
		return mRigidBody;
	}

	const ShapesEngine::ThreeDimensionalShape& RigidCylinder::GetShape() const
	{
		return mShape;
	}

	ShapesEngine::ThreeDimensionalShape& RigidCylinder::GetShape()
	{
		return mShape;
	}

	void RigidCylinder::SetPosition(const vec3& position)
	{
		mShape.position = position;
		mRigidBody.SetCenterOfMass(position + mOffset);
	}

	void RigidCylinder::UpdateModelMatrix()
	{
		mShape.position = mRigidBody.GetCenterOfMass() - mOffset;
		mShape.orientation = mRigidBody.GetOrientation();

		mShape.modelMatrix = MathEngine::Scale4x4(mRadius, mHeight, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float RigidCylinder::Volume()
	{
		return PI * mRadius * mRadius * mHeight;
	}
}