#pragma once

#include "RigidBody.h"
#include "ThreeDimensionalShape.h"
#include "BoundingVolume.h"
#include <memory>

namespace PhysicsEngine
{
	class RigidShape
	{
	public:

		/**brief Default Constructor.
		*/
		RigidShape();

		/**brief Creates a RigidShape object.
		*/
		RigidShape(float massDensity, const std::vector<ShapesEngine::Triangle>& triangles,
			std::unique_ptr<ShapesEngine::ThreeDimensionalShapeAbstract> shape,
			std::unique_ptr<PhysicsEngine::BoundingVolumeAbstract> boundingVolume);

		/**brief Initializes a RigidShape object.
		*/
		void InitializeRigidShape(float massDensity,const std::vector<ShapesEngine::Triangle>& triangles,
			std::unique_ptr<ShapesEngine::ThreeDimensionalShapeAbstract> shape,
			std::unique_ptr<PhysicsEngine::BoundingVolumeAbstract> boundingVolume);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------
		//Rigid Body Delegates

		/**brief Returns the mass of the RigidShape.
		*/
		float GetMass() const;

		/**brief Returns the inverse mass of the RigidShape.
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

		/**brief Returns the center of mass of the RigidShape.
		*/
		const vec3& GetCenterOfMass() const;

		/**brief Returns the linear velocity of the RigidShape.
		*/
		const vec3& GetLinearVelocity() const;

		/**brief Returns the linear momentum of the RigidShape.
		*/
		const vec3& GetLinearMomentum() const;

		/**brief Returns the orientaiton quaternion of the RigidShape.
		*/
		const MathEngine::Quaternion& GetOrientation() const;

		/**brief Returns the angular velocity of the RigidShape.
		*/
		const vec3& GetAngularVelocity() const;

		/**brief Returns the angular momentum of the RigidShape.
		*/
		const vec3& GetAngularMomentum() const;

		/**brief Sets the mass of the RigidShape to the specified float.
		*
		* If you want the RigidShape to have infinite mass, so it can't be moved, pass in 0.0f for the mass and the inverse mass will be set to 0.0f
		* to indicate infinite mass.
		*
		* If the specified mass is negative, the mass and inverse mass will be set to 0.0f.
		*/
		void SetMass(float mass);

		/**brief Sets the center of mass the RigidShape to the specified vector.
		*/
		void SetCenterOfMass(const vec3& centerOfMass);

		/**brief Sets the linear velocity of the RigidShape to the specified vector.
		*/
		void SetLinearVelocity(const vec3& velocity);

		/**brief Sets the linear momentum of the RigidShape to the specified vector.
		*/
		void SetLinearMomentum(const vec3& linearMomentum);

		/**brief Sets the body inertia tensor to the specified matrix.
		*/
		void SetBodyInertiaTensor(const mat3& bodyInertia);

		/**brief Sets the angular velocity of the RigidShape to the specified vector.
		*/
		void SetAngularVelocity(const vec3& angularVelocity);

		/**brief Sets the angular momentum of the RigidShape to the specified vector.
		*/
		void SetAngularMomentum(const vec3& angularMomentum);

		/**brief Sets the orientation of the RigidShape to the specified quaternion.
		*/
		void SetOrientation(const MathEngine::Quaternion& orientation);

		/**brief A numerical integrator using semi-implicit Euler method.
		* Uses semi-implicit Euler method to compute the new position and orientation of a rigid body.
		*/
		void Integrate(const vec3& netForce, const vec3& netTorque, float dt);

		//-------------------------------------------------------------------------------------------------------------------------------------------------------


		//-------------------------------------------------------------------------------------------------------------------------------------------------------
		//ThreeDimensionalShapeAbstract Delegates

		/**@brief Updates the RigidShapes model matrix.
		*/
		 void UpdateModelMatrix();

		/**@brief Returns the RigidShapes volume.
		*/
		 float Volume();

		/**@brief Returns the color of the RigidShape.
		*/
		 const RenderingEngine::Color& GetColor() const;

		/**@brief Returns the draw arguments of the RigidShape for rendering.
		*/
		 const RenderingEngine::DrawArguments& GetDrawArguments() const;

		/**@brief Returns the model matrix of the RigidShape.
		*/
		 const mat4& GetModelMatrix() const;

		/**@brief Returns the position of the RigidShape.
		*/
		 const vec3& GetPosition() const;

		 /**@brief Returns the dimensions of the RigidShape.
		*/
		 vec3 GetDimensions() const;

		 /**@brief Sets the dimensions of the RigidShape.
		 */
		 void SetDimensions(const vec3& dimensions);

		/**@brief Sets the position of the RigidShape.
		*/
		 void SetPosition(const vec3& position);

		/**@brief Sets the color of the RigidShape.
		*/
		 void SetColor(const RenderingEngine::Color& color);

		/**@brief Sets the draw arguments of the RigidShape.
		*/
		 void SetDrawArguments(const RenderingEngine::DrawArguments& drawArgs);
		
		//------------------------------------------------------------------------------------------------------------------------------------------------------



		//-------------------------------------------------------------------------------------------------------------------------------------------------------
		//Bounding Volume Delegates

		 /**brief Returns the model matrix of the RigidShapes bounding volume.
		 */
		 const mat4& GetBoundingVolumeModelMatrix() const;

		 /**brief Returns the draw arguments of the RigidShapes bounding volume.
		 */
		 const RenderingEngine::DrawArguments& GetBoundingVolumeDrawArguments() const;

		 /**brief Returns the color of the RigidShapes bounding volume.
		 */
		 const RenderingEngine::Color& GetBoundingVolumeColor() const;

		 /**brief Sets the draw arguments of the RigidShapes bounding volume for rendering.
		 */
		 void SetBoundingVolumeDrawArguments(const RenderingEngine::DrawArguments& drawArgs);

		 /**brief Sets the colr of the RigidShapes bounding volume.
		 */
		 void SetBoundingVolumeColor(const RenderingEngine::Color& color);

		 //------------------------------------------------------------------------------------------------------------------------------------------------------

	private:
		std::unique_ptr<ShapesEngine::ThreeDimensionalShapeAbstract> mShape;
		RigidBody mRigidBody;
		vec3 mOffset;

		std::unique_ptr<PhysicsEngine::BoundingVolumeAbstract> mBoundingVolume;
	};

	/**@brief Simulates the current RigidShape.
	*/
	void SimulateRigidShape(RigidShape& previousRigidShape, RigidShape& currentRigidShape, const vec3& netForce, const vec3& netTorque, float simulationTime);

	/**brief Interpolates the center of mass and orientation between r1 and r2 and stores the interpolated rigid shape in r3.
	*/
	void Interpolate(const RigidShape& r1, const RigidShape& r2, RigidShape& r3, float t);
};