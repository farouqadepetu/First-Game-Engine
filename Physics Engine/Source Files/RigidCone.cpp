#include "RigidCone.h"

namespace PhysicsEngine
{
	//-----------------------------------------------------------------------------------------------------------------------------
	//RigidCone Properties

	RigidCone::RigidCone()
	{}

	void RigidCone::InitializeRigidCone(float radius, float height, float massDensity,
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

	float RigidCone::GetRadius() const
	{
		return mRadius;
	}

	float RigidCone::GetHeight() const
	{
		return mHeight;
	}

	void RigidCone::SetRadius(float radius)
	{
		mRadius = radius;
	}

	void RigidCone::SetHeight(float height)
	{
		mHeight = height;
	}

	const RigidBody& RigidCone::GetRigidBody() const
	{
		return mRigidBody;
	}

	RigidBody& RigidCone::GetRigidBody()
	{
		return mRigidBody;
	}

	const ShapesEngine::ThreeDimensionalShape& RigidCone::GetShape() const
	{
		return mShape;
	}

	ShapesEngine::ThreeDimensionalShape& RigidCone::GetShape()
	{
		return mShape;
	}

	void RigidCone::SetPosition(const vec3& position)
	{
		mShape.position = position;
		mRigidBody.SetCenterOfMass(position + mOffset);
	}

	void RigidCone::UpdateModelMatrix()
	{
		mShape.position = mRigidBody.GetCenterOfMass() - mOffset;
		mShape.orientation = mRigidBody.GetOrientation();

		mShape.modelMatrix = MathEngine::Scale4x4(mRadius, mHeight, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float RigidCone::Volume()
	{
		return PI * mRadius * mRadius * (mHeight / 3.0f);
	}
}