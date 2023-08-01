#include "RigidBody.h"
#include <Windows.h>
#include <string>

namespace PhysicsEngine
{
	RigidBody::RigidBody() : mMass{ 1.0f }, mInverseMass{ 1.0f }, mCenterOfMass{ 0.0f, 0.0f, 0.0f },
		mLinearVelocity{ 0.0f, 0.0f, 0.0f }, mLinearMomentum{ 0.0f, 0.0f, 0.0f }, mNetForce{ 0.0f, 0.0f, 0.0f },
		mOrientation{ 1.0f, 0.0f, 0.0f, 0.0f }, mAngularVelocity{ 0.0f, 0.0f, 0.0f }, mAngularMomentum{ 0.0f, 0.0f, 0.0f }
	{}

	void RigidBody::InitializeRigidBody(float massDensity, const MathEngine::Quaternion& initialOrientation, 
		const std::vector<ShapesEngine::Triangle>& triangles, const mat3& scale)
	{
		if (massDensity <= 0.0f)
		{
			mMass = 0.0f;
			mInverseMass = 0.0f;

			SetOrientation(initialOrientation);
		}
		else
		{
			double mass{ 0.0 };
			vec3 cm;
			mat3 bodyInertiaTensor;

			ComputeMassProperties(triangles, mass, cm, bodyInertiaTensor, scale);

			SetMass((float)(mass * massDensity));

			mCenterOfMass = cm;

			SetOrientation(initialOrientation);
			SetBodyInertiaTensor(massDensity * bodyInertiaTensor);
		}

		SetLinearMomentum(vec3{ 0.0f, 0.0f, 0.0f });
		SetAngularMomentum(vec3{ 0.0f, 0.0f, 0.0f });
		mNetForce = vec3{ 0.0f, 0.0f, 0.0f };
		mNetTorque = vec3{ 0.0f, 0.0f, 0.0f };
	}

	float RigidBody::GetMass() const
	{
		return mMass;
	}

	float RigidBody::GetInverseMass() const
	{
		return mInverseMass;
	}

	const MathEngine::Matrix3x3& RigidBody::GetBodyInertiaTensor() const
	{
		return mBodyInertiaTensor;
	}

	const MathEngine::Matrix3x3& RigidBody::GetInverseBodyInertiaTensor() const
	{
		return mInverseBodyInertiaTensor;
	}

	const vec3& RigidBody::GetCenterOfMass() const
	{
		return mCenterOfMass;
	}

	const vec3& RigidBody::GetLinearVelocity() const
	{
		return mLinearVelocity;
	}

	const vec3& RigidBody::GetLinearMomentum() const
	{
		return mLinearMomentum;
	}
	const MathEngine::Quaternion& RigidBody::GetOrientation() const
	{
		return mOrientation;
	}

	const vec3& RigidBody::GetAngularVelocity() const
	{
		return mAngularVelocity;
	}

	const vec3& RigidBody::GetAngularMomentum() const
	{
		return mAngularMomentum;
	}

	const vec3& RigidBody::GetNetForce() const
	{
		return mNetForce;
	}

	const vec3& RigidBody::GetNetTorque() const
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

	void RigidBody::SetCenterOfMass(const vec3& centerOfMass)
	{
		mCenterOfMass = centerOfMass;
	}

	void RigidBody::SetLinearVelocity(const vec3& velocity)
	{
		if (mInverseMass > 0.0f)
		{
			mLinearVelocity = velocity;
			mLinearMomentum = mMass * velocity;
		}
	}

	void RigidBody::SetLinearMomentum(const vec3& linearMomentum)
	{
		if (mInverseMass > 0.0f)
		{
			mLinearMomentum = linearMomentum;
			mLinearVelocity = mLinearMomentum * mInverseMass;
		}
	}

	void RigidBody::SetBodyInertiaTensor(const MathEngine::Matrix3x3& bodyInertia)
	{
		mBodyInertiaTensor = bodyInertia;
		mInverseBodyInertiaTensor = Inverse(mBodyInertiaTensor);

		MathEngine::Matrix3x3 rOrientation(QuaternionToRotationMatrixRow3x3(mOrientation));
		mWorldCMInertiaTensor = rOrientation * mBodyInertiaTensor * Transpose(rOrientation);
		mInverseWorldCMInertiaTensor = Inverse(mWorldCMInertiaTensor);
	}

	void RigidBody::SetOrientation(const MathEngine::Quaternion& orientation)
	{
		mOrientation = Normalize(orientation);

		if (mInverseMass > 0)
		{
			MathEngine::Matrix3x3 rOrientation(QuaternionToRotationMatrixRow3x3(mOrientation));
			mWorldCMInertiaTensor = rOrientation * mBodyInertiaTensor * Transpose(rOrientation);
			mInverseWorldCMInertiaTensor = Inverse(mWorldCMInertiaTensor);
		}
	}

	void RigidBody::SetAngularVelocity(const vec3& angularVelocity)
	{
		if (mInverseMass > 0)
		{
			mAngularVelocity = angularVelocity;
			mAngularMomentum = angularVelocity * mWorldCMInertiaTensor;
		}
	}

	void RigidBody::SetAngularMomentum(const vec3& angularMomentum)
	{
		if (mInverseMass > 0)
		{
			mAngularMomentum = angularMomentum;

			mAngularVelocity = angularMomentum * mInverseWorldCMInertiaTensor;
		}
	}

	void RigidBody::ResetForce()
	{
		mNetForce = vec3{ 0.0f, 0.0f, 0.0f };
	}

	void RigidBody::ResetTorque()
	{
		mNetTorque = vec3{ 0.0f, 0.0f, 0.0f };
	}

	void RigidBody::AddForce(const vec3& force)
	{
		mNetForce += force;
	}

	void RigidBody::AddTorque(const vec3& force, const vec3& point)
	{
		mNetTorque += MathEngine::CrossProduct(force, point - mCenterOfMass);
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

			mAngularVelocity = mAngularMomentum * mInverseWorldCMInertiaTensor;

			MathEngine::Quaternion dqdt = MathEngine::Quaternion{ 0.0f, mAngularVelocity } * mOrientation * 0.5f;

			mOrientation += dqdt * dt;

			mOrientation = Normalize(mOrientation);
		}
	}

	void RigidBody::Integrate(const vec3& netForce, const vec3& netTorque, float dt)
	{
		//If inverse mass equals to 0 that means the rigid body has infinite mass and cannot be moved.
		if (mInverseMass > 0.0f)
		{
			mLinearMomentum += netForce * dt;

			mLinearVelocity = mLinearMomentum * mInverseMass;

			mCenterOfMass += mLinearVelocity * dt;


			mAngularMomentum += netTorque * dt;

			mAngularVelocity = mAngularMomentum * mInverseWorldCMInertiaTensor;

			MathEngine::Quaternion dqdt = MathEngine::Quaternion{ 0.0f, mAngularVelocity } * mOrientation * 0.5f;

			mOrientation += dqdt * dt;

			mOrientation = Normalize(mOrientation);
		}
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//NON-MEMBER FUNCTIONS

	void Interpolate(const RigidBody& r1, const RigidBody& r2, RigidBody& r3, float t)
	{
		r3.SetCenterOfMass(MathEngine::Lerp(r1.GetCenterOfMass(), r2.GetCenterOfMass(), t));

		r3.SetOrientation(MathEngine::Slerp(r1.GetOrientation(), r2.GetOrientation(), t));
	}

	//---------------------------------------------------------------------------------------------------------------------------
}