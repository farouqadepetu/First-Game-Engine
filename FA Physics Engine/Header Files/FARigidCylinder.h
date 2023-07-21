#pragma once

#include "FARigidBody.h"
#include "FAThreeDimensionalShape.h"

namespace FAPhysicsShapes
{
	class RigidCylinder
	{
	public:

		/**@brief Default Constructor.
		* Constructs a RigidCylinder object.
		*/
		RigidCylinder();

		/**@brief Iniitalizes a rigid cylinder that can be used to do physics simulations.
		*
		* @param[in] radius The radius of the cylinder.
		* @param[in] height The height of the cylinder.
		* @param[in] color The color of the cylinder.
		* @param[in] massDensity The mass density of the cylinder.
		* @param[in] initialPosition The initial position of the cylinder.
		* @param[in] initialOrientation The initial orientation of the cylinder.
		* @param[in] color The color of the cylinder.
		* @param[in] vertices The vertex list of a unit cylinder.
		* @param[in] triangles The triangle list of a unit cylinder.
		*/
		void InitializeRigidCylinder(float radius, float height, float massDensity,
			const FAMath::Vector4D& initialPosition, const FAMath::Quaternion& initialOrientation, const FAColor::Color& color,
			const std::vector<FAShapes::Vertex>& vertices, const std::vector<FAShapes::Triangle>& triangles);

		/**@brief Returns the radius of the cylinder.
		*/
		float GetRadius() const;

		/**@brief Returns the height of the cylinder.
		*/
		float GetHeight() const;

		/**@brief Sets the radius of the cylinder.
		*/
		void SetRadius(float radius);

		/**@brief Sets the height of the cylinder.
		*/
		void SetHeight(float height);

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

		/**@brief Sets the position of the RigidCylinder.
		*/
		void SetPosition(const FAMath::Vector4D& position);

		/**@brief Updates the model matrix of the RigidCylinder.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the cylinder.
		*/
		float Volume();

	private:
		float mRadius;
		float mHeight;

		FAPhysics::RigidBody mRigidBody;
		FAShapes::ThreeDimensionalShape mShape;
		FAMath::Vector4D mOffset;
	};
}