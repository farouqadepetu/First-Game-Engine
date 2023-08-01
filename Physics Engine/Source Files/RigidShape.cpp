#include "RigidShape.h"

namespace PhysicsEngine
{
	RigidShape::RigidShape()
	{}

	RigidShape::RigidShape(float massDensity, const std::vector<ShapesEngine::Triangle>& triangles,
		std::unique_ptr<ShapesEngine::ThreeDimensionalShapeAbstract> shape,
		std::unique_ptr<PhysicsEngine::BoundingVolumeAbstract> boundingVolume)
	{
		InitializeRigidShape(massDensity, triangles, std::move(shape), std::move(boundingVolume));
	}

	void RigidShape::InitializeRigidShape(float massDensity, const std::vector<ShapesEngine::Triangle>& triangles,
		std::unique_ptr<ShapesEngine::ThreeDimensionalShapeAbstract> shape,
		std::unique_ptr<PhysicsEngine::BoundingVolumeAbstract> boundingVolume)
	{
		mShape = std::move(shape);
		mBoundingVolume = std::move(boundingVolume);
		mBoundingVolume->SetPosition(mShape->GetPosition());

		mRigidBody.InitializeRigidBody(massDensity, mShape->GetOrientation(), triangles, MathEngine::Scale(mShape->GetDimensions()));

		//Formula used is:
		//local origin world location = center of mass world locaiton - offset at t = 0
		//so offset = center of mass world locaiton - local origin world location at t = 0.
		//At t = 0 the local origin world location is (0, 0, 0) so offset = center of mass world locaiton.
		mOffset = mRigidBody.GetCenterOfMass();

		//Move the center of mass to its initial location in the world.
		mRigidBody.SetCenterOfMass(mShape->GetPosition() + mOffset);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------
	//Rigid Body Delegates

	float RigidShape::GetMass() const
	{
		return mRigidBody.GetMass();
	}

	float RigidShape::GetInverseMass() const
	{
		return mRigidBody.GetInverseMass();
	}

	const MathEngine::Matrix3x3& RigidShape::GetBodyInertiaTensor() const
	{
		return mRigidBody.GetBodyInertiaTensor();
	}

	const MathEngine::Matrix3x3& RigidShape::GetInverseBodyInertiaTensor() const
	{
		return mRigidBody.GetInverseBodyInertiaTensor();
	}

	const vec3& RigidShape::GetCenterOfMass() const
	{
		return mRigidBody.GetCenterOfMass();;
	}

	const vec3& RigidShape::GetLinearVelocity() const
	{
		return mRigidBody.GetLinearVelocity();
	}

	const vec3& RigidShape::GetLinearMomentum() const
	{
		return mRigidBody.GetLinearMomentum();
	}
	const MathEngine::Quaternion& RigidShape::GetOrientation() const
	{
		return mRigidBody.GetOrientation();
	}

	const vec3& RigidShape::GetAngularVelocity() const
	{
		return mRigidBody.GetAngularVelocity();
	}

	const vec3& RigidShape::GetAngularMomentum() const
	{
		return mRigidBody.GetAngularMomentum();
	}

	void RigidShape::SetMass(float mass)
	{
		mRigidBody.SetMass(mass);
	}

	void RigidShape::SetCenterOfMass(const vec3& centerOfMass)
	{
		mShape->SetPosition(centerOfMass - mOffset);
		mRigidBody.SetCenterOfMass(centerOfMass);

		mBoundingVolume->SetPosition(mShape->GetPosition());
	}

	void RigidShape::SetLinearVelocity(const vec3& velocity)
	{
		mRigidBody.SetLinearVelocity(velocity);
	}

	void RigidShape::SetLinearMomentum(const vec3& linearMomentum)
	{
		mRigidBody.SetLinearMomentum(linearMomentum);
	}

	void RigidShape::SetBodyInertiaTensor(const MathEngine::Matrix3x3& bodyInertia)
	{
		mRigidBody.SetBodyInertiaTensor(bodyInertia);
	}

	void RigidShape::SetOrientation(const MathEngine::Quaternion& orientation)
	{
		mRigidBody.SetOrientation(orientation);
	}

	void RigidShape::SetAngularVelocity(const vec3& angularVelocity)
	{
		mRigidBody.SetAngularVelocity(angularVelocity);
	}

	void RigidShape::SetAngularMomentum(const vec3& angularMomentum)
	{
		mRigidBody.SetAngularMomentum(angularMomentum);
	}

	void RigidShape::Integrate(const vec3& netForce, const vec3& netTorque, float dt)
	{
		mRigidBody.Integrate(netForce, netTorque, dt);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------------------------------------------------------------------------
	//RigidShape Delegates

	void RigidShape::UpdateModelMatrix()
	{
		mShape->SetPosition(mRigidBody.GetCenterOfMass() - mOffset);
		mShape->SetOrientation(mRigidBody.GetOrientation());
		mBoundingVolume->SetPosition(mShape->GetPosition());
		
		mShape->UpdateModelMatrix();

		mBoundingVolume->TransformBoundingVolume(mShape->GetModelMatrix());

		mBoundingVolume->UpdateModelMatrix();
	}

	float RigidShape::Volume()
	{
		return mShape->Volume();
	}

	const RenderingEngine::Color& RigidShape::GetColor() const
	{
		return mShape->GetColor();
	}

	const RenderingEngine::DrawArguments& RigidShape::GetDrawArguments() const
	{
		return mShape->GetDrawArguments();
	}

	const mat4& RigidShape::GetModelMatrix() const
	{
		return mShape->GetModelMatrix();
	}

	const vec3& RigidShape::GetPosition() const
	{
		return mShape->GetPosition();
	}

	vec3 RigidShape::GetDimensions() const
	{
		return mShape->GetDimensions();
	}

	void RigidShape::SetDimensions(const vec3& dimensions)
	{
		mShape->SetDimensions(dimensions);
	}

	void RigidShape::SetPosition(const vec3& position)
	{
		mRigidBody.SetCenterOfMass(position + mOffset);
		mShape->SetPosition(position);
		mBoundingVolume->SetPosition(position);
	}

	void RigidShape::SetColor(const RenderingEngine::Color& color)
	{
		mShape->SetColor(color);
	}

	void RigidShape::SetDrawArguments(const RenderingEngine::DrawArguments& drawArgs)
	{
		mShape->SetDrawArguments(drawArgs);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------------------------------------
	//Bounding Volume Delegates

	const mat4& RigidShape::GetBoundingVolumeModelMatrix() const
	{
		return mBoundingVolume->GetModelMatrix();
	}

	const RenderingEngine::DrawArguments& RigidShape::GetBoundingVolumeDrawArguments() const
	{
		return mBoundingVolume->GetDrawArguments();
	}

	const RenderingEngine::Color& RigidShape::GetBoundingVolumeColor() const
	{
		return mBoundingVolume->GetColor();
	}

	void RigidShape::SetBoundingVolumeDrawArguments(const RenderingEngine::DrawArguments& drawArgs)
	{
		mBoundingVolume->SetDrawArguments(drawArgs);
	}

	void RigidShape::SetBoundingVolumeColor(const RenderingEngine::Color& color)
	{
		mBoundingVolume->SetColor(color);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------



	//-------------------------------------------------------------------------------------------------------------------------------------------------------

	//NON-MEMBER FUNCTIONS

	void SimulateRigidShape(RigidShape& previousRigidShape, RigidShape& currentRigidShape, const vec3& netForce, const vec3& netTorque, float simulationTime)
	{
		previousRigidShape.SetCenterOfMass(currentRigidShape.GetCenterOfMass());
		previousRigidShape.SetOrientation(currentRigidShape.GetOrientation());

		currentRigidShape.Integrate(netForce, netTorque, simulationTime);
	}

	void Interpolate(const RigidShape& r1, const RigidShape& r2, RigidShape& r3, float t)
	{
		r3.SetCenterOfMass(MathEngine::Lerp(r1.GetCenterOfMass(), r2.GetCenterOfMass(), t));

		r3.SetOrientation(MathEngine::Slerp(r1.GetOrientation(), r2.GetOrientation(), t));
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------

};