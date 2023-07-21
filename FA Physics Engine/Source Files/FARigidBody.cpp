#include "FARigidBody.h"

namespace FAPhysics
{
	RigidBody::RigidBody() : mMass{ 1.0f }, mInverseMass{ 1.0f }, mCenterOfMass{ 0.0f, 0.0f, 0.0f, 1.0f },
		mLinearVelocity{ 0.0f, 0.0f, 0.0f, 0.0f }, mLinearMomentum{ 0.0f, 0.0f, 0.0f, 0.0f }, mNetForce{ 0.0f, 0.0f, 0.0f, 0.0f },
		mOrientation{ 1.0f, 0.0f, 0.0f, 0.0f }, mAngularVelocity{ 0.0f, 0.0f, 0.0f, 0.0f }, mAngularMomentum{ 0.0f, 0.0f, 0.0f, 0.0f }
	{}

	void RigidBody::InitializeRigidBody(float massDensity, const FAMath::Quaternion& initialOrientation, const std::vector<FAShapes::Triangle>& triangles)
	{
		if (massDensity <= 0.0f)
		{
			mMass = 0.0f;
			mInverseMass = 0.0f;
		}
		else
		{
			double mass{ 0.0 };
			FAMath::Vector4D cm;
			FAMath::Matrix3x3 bodyInertiaTensor;

			ComputeMassProperties(triangles, mass, cm, bodyInertiaTensor);

			SetMass((float)(mass * massDensity));

			mCenterOfMass = cm;

			SetOrientation(initialOrientation);
			SetBodyInertiaTensor(massDensity * bodyInertiaTensor);
		}

		SetLinearMomentum(FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f));
		SetAngularMomentum(FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f));
		mNetForce = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
		mNetTorque = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float RigidBody::GetMass() const
	{
		return mMass;
	}

	float RigidBody::GetInverseMass() const
	{
		return mInverseMass;
	}

	const FAMath::Matrix3x3& RigidBody::GetBodyInertiaTensor() const
	{
		return mBodyInertiaTensor;
	}

	const FAMath::Matrix3x3& RigidBody::GetInverseBodyInertiaTensor() const
	{
		return mInverseBodyInertiaTensor;
	}

	const FAMath::Vector4D& RigidBody::GetCenterOfMass() const
	{
		return mCenterOfMass;
	}

	const FAMath::Vector4D& RigidBody::GetLinearVelocity() const
	{
		return mLinearVelocity;
	}

	const FAMath::Vector4D& RigidBody::GetLinearMomentum() const
	{
		return mLinearMomentum;
	}
	const FAMath::Quaternion& RigidBody::GetOrientation() const
	{
		return mOrientation;
	}

	const FAMath::Vector4D& RigidBody::GetAngularVelocity() const
	{
		return mAngularVelocity;
	}

	const FAMath::Vector4D& RigidBody::GetAngularMomentum() const
	{
		return mAngularMomentum;
	}

	const FAMath::Vector4D& RigidBody::GetNetForce() const
	{
		return mNetForce;
	}

	const FAMath::Vector4D& RigidBody::GetNetTorque() const
	{
		return mNetTorque;
	}

	void RigidBody::SetMass(float mass)
	{
		if (mass <= 0.0f)
		{
			mMass = 0.0f;
			mInverseMass = 0.0f;
		}
		else
		{
			mMass = mass;
			mInverseMass = 1.0f / mass;
		}
	}

	void RigidBody::SetCenterOfMass(const FAMath::Vector4D& centerOfMass)
	{
		mCenterOfMass = centerOfMass;
	}

	void RigidBody::SetLinearVelocity(const FAMath::Vector4D& velocity)
	{
		if (mInverseMass > 0.0f)
		{
			mLinearVelocity = velocity;
			mLinearMomentum = mMass * velocity;
		}
	}

	void RigidBody::SetLinearMomentum(const FAMath::Vector4D& linearMomentum)
	{
		if (mInverseMass > 0.0f)
		{
			mLinearMomentum = linearMomentum;
			mLinearVelocity = mLinearMomentum * mInverseMass;
		}
	}

	void RigidBody::SetBodyInertiaTensor(const FAMath::Matrix3x3& bodyInertia)
	{
		mBodyInertiaTensor = bodyInertia;
		mInverseBodyInertiaTensor = Inverse(mBodyInertiaTensor);

		FAMath::Matrix4x4 rOrientation(QuaternionToRotationMatrixRow(mOrientation));
		mWorldCMInertiaTensor = rOrientation * FAMath::Matrix4x4(mBodyInertiaTensor) * Transpose(rOrientation);
		mInverseWorldCMInertiaTensor = Inverse(mWorldCMInertiaTensor);
	}

	void RigidBody::SetOrientation(const FAMath::Quaternion& orientation)
	{
		mOrientation = Normalize(orientation);

		if (mInverseMass > 0)
		{
			FAMath::Matrix4x4 rOrientation = QuaternionToRotationMatrixRow(mOrientation);
			mWorldCMInertiaTensor = rOrientation * FAMath::Matrix4x4(mBodyInertiaTensor) * Transpose(rOrientation);
			mInverseWorldCMInertiaTensor = Inverse(mWorldCMInertiaTensor);
		}
	}

	void RigidBody::SetAngularVelocity(const FAMath::Vector4D& angularVelocity)
	{
		if (mInverseMass > 0)
		{
			mAngularVelocity = angularVelocity;
			mAngularMomentum = angularVelocity * FAMath::Matrix4x4(mWorldCMInertiaTensor) ;
		}
	}

	void RigidBody::SetAngularMomentum(const FAMath::Vector4D& angularMomentum)
	{
		if (mInverseMass > 0)
		{
			mAngularMomentum = angularMomentum;

			mAngularVelocity = angularMomentum * FAMath::Matrix4x4(mInverseWorldCMInertiaTensor);
		}
	}

	void RigidBody::ResetForce()
	{
		mNetForce = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void RigidBody::ResetTorque()
	{
		mNetTorque = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void RigidBody::AddForce(const FAMath::Vector4D& force)
	{
		mNetForce += force;
	}

	void RigidBody::AddTorque(const FAMath::Vector4D& force, const FAMath::Vector4D& point)
	{
		mNetTorque += FAMath::CrossProduct(force, point - mCenterOfMass);
	}

	void RigidBody::Integrate(float dt)
	{
		//If inverse mass equals to 0 that means the rigid body has infinite mass and cannot be moved.
		if (mInverseMass > 0.0f)
		{
			mLinearMomentum += mNetForce * dt;

			mLinearVelocity = mLinearMomentum * mInverseMass;

			mCenterOfMass += mLinearVelocity * dt;

			mAngularMomentum += mNetTorque * dt;

			mAngularVelocity = mAngularMomentum * FAMath::Matrix4x4(mInverseWorldCMInertiaTensor);

			FAMath::Quaternion dqdt = FAMath::Quaternion(0.0f, mAngularVelocity) * mOrientation * 0.5f;

			mOrientation += dqdt * dt;

			mOrientation = Normalize(mOrientation);
		}
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//NON-MEMBER FUNCTIONS

	void Interpolate(const RigidBody& r1, const RigidBody& r2, RigidBody& r3, float t)
	{
		r3.SetCenterOfMass(FAMath::Lerp(r1.GetCenterOfMass(), r2.GetCenterOfMass(), t));

		r3.SetOrientation(FAMath::Slerp(r1.GetOrientation(), r2.GetOrientation(), t));
	}

	//---------------------------------------------------------------------------------------------------------------------------
}