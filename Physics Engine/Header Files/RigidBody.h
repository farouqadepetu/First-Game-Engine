#pragma once

#include "PolyhedralMassProperties.h"

/** @namespace PhysicsEngine
* @brief An engine for physics simulations.
*/
namespace PhysicsEngine
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
		* to indicate infinite mass.\n
		*
		* If the specified mass density is negative, the mass and inverse mass will be set to 0.0f.\n
		*
		* Computes the center of mass and inertia tensors from the given triangles that make up a solid polyhedron if the object does not have infinite mass.\n
		* 
		* Stores the local space bounding volumes used for collision detection.
		*/
		void InitializeRigidBody(float massDensity, const MathEngine::Quaternion& initialOrientation, 
			const std::vector<ShapesEngine::Triangle>& triangles, const mat3& scale);

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
		const mat3& GetBodyInertiaTensor() const;

		/**brief Returns the inverse of the inertia tensor in body coordinates.
		*/
		const mat3& GetInverseBodyInertiaTensor() const;

		/**brief Returns the center of mass of the rigid body.
		*/
		const vec3& GetCenterOfMass() const;

		/**brief Returns the linear velocity of the rigid body.
		*/
		const vec3& GetLinearVelocity() const;

		/**brief Returns the linear momentum of the rigid body.
		*/
		const vec3& GetLinearMomentum() const;

		/**brief Returns the orientaiton quaternion of the rigid body.
		*/
		const MathEngine::Quaternion& GetOrientation() const;

		/**brief Returns the angular velocity of the rigid body.
		*/
		const vec3& GetAngularVelocity() const;

		/**brief Returns the angular momentum of the rigid body.
		*/
		const vec3& GetAngularMomentum() const;

		/**brief Returns the net force acting on the rigid body.
		*/
		const vec3& GetNetForce() const;

		/**brief Returns the net torque acting on the rigid body.
		*/
		const vec3& GetNetTorque() const;

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
		void SetCenterOfMass(const vec3& centerOfMass);

		/**brief Sets the linear velocity of the rigid body to the specified vector.
		*/
		void SetLinearVelocity(const vec3& velocity);

		/**brief Sets the linear momentum of the rigid body to the specified vector.
		*/
		void SetLinearMomentum(const vec3& linearMomentum);

		/**brief Sets the body inertia tensor to the specified matrix.
		*/
		void SetBodyInertiaTensor(const mat3& bodyInertia);

		/**brief Sets the angular velocity of the rigid body to the specified vector.
		*/
		void SetAngularVelocity(const vec3& angularVelocity);

		/**brief Sets the angular momentum of the rigid body to the specified vector.
		*/
		void SetAngularMomentum(const vec3& angularMomentum);

		/**brief Sets the orientation of the rigid body to the specified quaternion.
		*/
		void SetOrientation(const MathEngine::Quaternion& orientation);

		/**brief Sets the net force of a rigid body to the zero vector.
		*/
		void ResetForce();

		/**brief Sets the net torque of a rigid body to the zero vector.
		*/
		void ResetTorque();

		/**brief Adds the specified force to the net force of a rigid body.
		*/
		void AddForce(const vec3& force);

		/**brief Adds the computed torque to the net torque.
		* Computes the torque being applied to the point using the equation torque = force x (point - center of mass).
		*/
		void AddTorque(const vec3& force, const vec3& point);

		/**brief A numerical integrator using semi-implicit Euler method.
		* Uses semi-implicit Euler method to compute the new position and orientation of a rigid body.
		*/
		void Integrate(float dt);

		/**brief A numerical integrator using semi-implicit Euler method.
		* Uses semi-implicit Euler method to compute the new position and orientation of a rigid body.
		*/
		void Integrate(const vec3& netForce, const vec3& netTorque, float dt);

	private:
		float mMass;
		float mInverseMass;

		mat3 mBodyInertiaTensor;
		mat3 mInverseBodyInertiaTensor;
		mat3 mWorldCMInertiaTensor;
		mat3 mInverseWorldCMInertiaTensor;

		vec3 mCenterOfMass;
		vec3 mLinearVelocity;
		vec3 mLinearMomentum;
		vec3 mNetForce;

		MathEngine::Quaternion mOrientation;
		vec3 mAngularVelocity;
		vec3 mAngularMomentum;
		vec3 mNetTorque;
	};

	/**brief Interpolates the center of mass and orientation between r1 and r2 and stores the interpolated rigid body in r3.
	*/
	void Interpolate(const RigidBody& r1, const RigidBody& r2, RigidBody& r3, float t);
}