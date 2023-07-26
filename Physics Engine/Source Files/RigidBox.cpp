#include "RigidBox.h"

namespace PhysicsEngine
{
	//-----------------------------------------------------------------------------------------------------------------------------
	//RigidBox Properties
	RigidBox::RigidBox() : mWidth{ 1.0f }, mHeight{ 1.0f }, mDepth{ 1.0f }
	{}

	void RigidBox::InitializeRigidBox(float width, float height, float depth, float massDensity,
		const vec3& initialPosition, const MathEngine::Quaternion& initialOrientation, const RenderingEngine::Color& color,
		const std::vector<ShapesEngine::Vertex>& vertices, const std::vector<ShapesEngine::Triangle>& triangles)
	{
		mWidth = width;
		mHeight = height;
		mDepth = depth;

		mShape.position = initialPosition;
		mShape.orientation = initialOrientation;
		mShape.color = color;

		//Need to scale the vertices to get it to the proper size so the computed mass is dependent on the volume of the object.
		std::vector<ShapesEngine::Vertex> tempVertexList = vertices;
		std::vector<ShapesEngine::Triangle> tempTriangleList = triangles;
		MathEngine::Matrix3x3 scale(MathEngine::Scale(width, height, depth));

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
	float RigidBox::GetWidth() const
	{
		return mWidth;
	}

	float RigidBox::GetHeight() const
	{
		return mHeight;
	}

	float RigidBox::GetDepth() const
	{
		return mDepth;
	}

	void RigidBox::SetWidth(float width)
	{
		mWidth = width;
	}

	void RigidBox::SetHeight(float height)
	{
		mHeight = height;
	}

	void RigidBox::SetDepth(float depth)
	{
		mDepth = depth;
	}

	const RigidBody& RigidBox::GetRigidBody() const
	{
		return mRigidBody;
	}

	RigidBody& RigidBox::GetRigidBody()
	{
		return mRigidBody;
	}

	const ShapesEngine::ThreeDimensionalShape& RigidBox::GetShape() const
	{
		return mShape;
	}

	ShapesEngine::ThreeDimensionalShape& RigidBox::GetShape()
	{
		return mShape;
	}

	void RigidBox::SetPosition(const vec3& position)
	{
		mShape.position = position;
		mRigidBody.SetCenterOfMass(position + mOffset);
	}

	void RigidBox::UpdateModelMatrix()
	{
		mShape.position = mRigidBody.GetCenterOfMass() - mOffset;
		mShape.orientation = mRigidBody.GetOrientation();

		mShape.modelMatrix = MathEngine::Scale4x4(mWidth, mHeight, mDepth) * MathEngine::QuaternionToRotationMatrixRow4x4(mShape.orientation) *
			MathEngine::Translate(mShape.position);
	}

	float RigidBox::Volume()
	{
		return mWidth * mHeight * mDepth;
	}
}