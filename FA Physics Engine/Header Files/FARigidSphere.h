#pragma once

#include "FARigidBody.h"
#include "FAThreeDimensionalShape.h"
#include "FAAABB.h"

namespace FAPhysicsShapes
{
	class RigidSphere
	{
	public:

		/**@brief Default Constructor.
		* Constructs a RigidSphere object.
		*/
		RigidSphere();

		/**@brief Iniitalizes a rigid sphere that can be used to do physics simulations.
		*
		* @param[in] radius The radius of the sphere.
		* @param[in] color The color of the sphere.
		* @param[in] massDensity The mass density of the sphere.
		* @param[in] initialPosition The initial position of the sphere.
		* @param[in] initialOrientation The initial orientation of the sphere.
		* @param[in] color The color of the sphere.
		* @param[in] vertices The vertex list of a unit sphere.
		* @param[in] triangles The triangle list of a unit sphere.
		*/
		void InitializeRigidSphere(float radius, float massDensity,
			const FAMath::Vector4D& initialPosition, const FAMath::Quaternion& initialOrientation, const FAColor::Color& color,
			const std::vector<FAShapes::Vertex>& vertices, const std::vector<FAShapes::Triangle>& triangles);

		/**@brief Returns the radius of the sphere.
		*/
		float GetRadius() const;

		/**@brief Returns the axis-aligned bounding box of the sphere.
		*/
		const FACollisions::AABB& GetBoundingBox() const;

		/**@brief Sets the radius of the sphere.
		*/
		void SetRadius(float radius);

		/**@brief Returns the RigidBody object.
		*/
		const FAPhysics::RigidBody& GetRigidBody() const;

		/**@brief Returns the RigidBody object.
		*/
		FAPhysics::RigidBody& GetRigidBody();

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const FAShapes::ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		FAShapes::ThreeDimensionalShape& GetShape();

		/**@brief Sets the position of the RigidSphere.
		*/
		void SetPosition(const FAMath::Vector4D& position);

		/**@brief Updates the model matrix of the RigidSphere.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the sphere.
		*/
		float Volume();

	private:
		float mRadius;

		FAPhysics::RigidBody mRigidBody;
		FAShapes::ThreeDimensionalShape mShape;
		FAMath::Vector4D mOffset;

		FACollisions::AABB mLocalBoundingBox;
		FACollisions::AABB mWorldBoundingBox;
	};
}