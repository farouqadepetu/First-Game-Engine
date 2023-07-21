#pragma once

#include "PolyhedralMassProperties.h"
#include <vector>

namespace FAPhysics
{
	class RigidBody
	{
	public:
		/**brief Default Constructor.
		* Initializes all scalar member variables to 1.0f and all vectors to the zero vector.
		*/
		RigidBody();

		/**brief Initializes the properties of a rigid body.
		*
		* If you want the rigid body to have infinite mass, so it can't be moved, pass in 0.0f for the mass density and the inverse mass will be set to 0.0f
		* to indicate infinite mass.
		*
		* If the specified mass density is negative, the mass and inverse mass will be set to 0.0f.
		*
		* Computes the center of mass and inertia tensors from the given triangles that make up a solid polyhedron if the object does not have infinite mass.
		*/
		void InitializeRigidBody(float massDensity, const FAMath::Quaternion& initialOrientation, const std::vector<FAShapes::Triangle>& triangles);

		/**brief Returns the mass of the rigid body.
		*/
		float GetMass() const;

		/**brief Returns the inverse mass of the rigid body.
		* 
		* If the inverse mass equals to 0 that means the mass is infinity.
		*/
		float GetInverseMass() const;

		/**brief Returns the inertia tensor in body coordinates.
		*/
		const FAMath::Matrix3x3& GetBodyInertiaTensor() const;

		/**brief Returns the inverse of the inertia tensor in body coordinates.
		*/
		const FAMath::Matrix3x3& GetInverseBodyInertiaTensor() const;

		/**brief Returns the center of mass of the rigid body.
		*/
		const FAMath::Vector4D& GetCenterOfMass() const;

		/**brief Returns the linear velocity of the rigid body.
		*/
		const FAMath::Vector4D& GetLinearVelocity() const;

		/**brief Returns the linear momentum of the rigid body.
		*/
		const FAMath::Vector4D& GetLinearMomentum() const;

		/**brief Returns the orientaiton quaternion of the rigid body.
		*/
		const FAMath::Quaternion& GetOrientation() const;

		/**brief Returns the angular velocity of the rigid body.
		*/
		const FAMath::Vector4D& GetAngularVelocity() const;

		/**brief Returns the angular momentum of the rigid body.
		*/
		const FAMath::Vector4D& GetAngularMomentum() const;

		/**brief Returns the net force acting on the rigid body.
		*/
		const FAMath::Vector4D& GetNetForce() const;

		/**brief Returns the net torque acting on the rigid body.
		*/
		const FAMath::Vector4D& GetNetTorque() const;
	
		/**brief Sets the mass of the rigid body to the specified float.
		* 
		* If you want the rigid body to have infinite mass, so it can't be moved, pass in 0.0f for the mass and the inverse mass will be set to 0.0f
		* to indicate infinite mass.
		* 
		* If the specified mass is negative, the mass and inverse mass will be set to 0.0f.
		*/
		void SetMass(float mass);
		
		/**brief Sets the center of mass the rigid body to the specified vector.
		*/
		void SetCenterOfMass(const FAMath::Vector4D& centerOfMass);

		/**brief Sets the linear velocity of the rigid body to the specified vector.
		*/
		void SetLinearVelocity(const FAMath::Vector4D& velocity);

		/**brief Sets the linear momentum of the rigid body to the specified vector.
		*/
		void SetLinearMomentum(const FAMath::Vector4D& linearMomentum);

		/**brief Sets the body inertia tensor to the specified matrix.
		*/
		void SetBodyInertiaTensor(const FAMath::Matrix3x3& bodyInertia);

		/**brief Sets the angular velocity of the rigid body to the specified vector.
		*/
		void SetAngularVelocity(const FAMath::Vector4D& angularVelocity);

		/**brief Sets the angular momentum of the rigid body to the specified vector.
		*/
		void SetAngularMomentum(const FAMath::Vector4D& angularMomentum);

		/**brief Sets the orientation of the rigid body to the specified quaternion.
		*/
		void SetOrientation(const FAMath::Quaternion& orientation);

		/**brief Sets the net force of a rigid body to the zero vector.
		*/
		void ResetForce();

		/**brief Sets the net torque of a rigid body to the zero vector.
		*/
		void ResetTorque();

		/**brief Adds the specified force to the net force of a rigid body.
		*/
		void AddForce(const FAMath::Vector4D& force);

		/**brief Adds the computed torque to the net torque.
		* Computes the torque being applied to the point using the equation torque = force x (point - center of mass).
		*/
		void AddTorque(const FAMath::Vector4D& force, const FAMath::Vector4D& point);

		/**brief A numerical integrator using semi-implicit Euler method.
		* Uses semi-implicit Euler method to compute the new position and orientation of a rigid body.
		*/
		void Integrate(float dt);

	private:
		float mMass;
		float mInverseMass;

		FAMath::Matrix3x3 mBodyInertiaTensor;
		FAMath::Matrix3x3 mInverseBodyInertiaTensor;
		FAMath::Matrix3x3 mWorldCMInertiaTensor;
		FAMath::Matrix3x3 mInverseWorldCMInertiaTensor;

		FAMath::Vector4D mCenterOfMass;
		FAMath::Vector4D mLinearVelocity;
		FAMath::Vector4D mLinearMomentum;
		FAMath::Vector4D mNetForce;

		FAMath::Quaternion mOrientation;
		FAMath::Vector4D mAngularVelocity;
		FAMath::Vector4D mAngularMomentum;
		FAMath::Vector4D mNetTorque;
	};

	/**brief Interpolates the center of mass and orientation between r1 and r2 and stores the interpolated rigid body in r3.
	*/
	void Interpolate(const RigidBody& r1, const RigidBody& r2, RigidBody& r3, float t);
}