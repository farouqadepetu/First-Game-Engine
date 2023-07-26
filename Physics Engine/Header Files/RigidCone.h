#pragma once

#include "RigidBody.h"
#include "ThreeDimensionalShape.h"

namespace PhysicsEngine
{
	class RigidCone
	{
	public:

		/**@brief Default Constructor.
		* Constructs a RigidCone object.
		*/
		RigidCone();

		/**@brief Iniitalizes a rigid cone that can be used to do physics simulations.
		*
		* @param[in] radius The radius of the cone.
		* @param[in] height The height of the cone.
		* @param[in] color The color of the cone.
		* @param[in] massDensity The mass density of the cone.
		* @param[in] initialPosition The initial position of the cone.
		* @param[in] initialOrientation The initial orientation of the cone.
		* @param[in] color The color of the cone.
		* @param[in] vertices The vertex list of a unit cone.
		* @param[in] triangles The triangle list of a unit cone.
		*/
		void InitializeRigidCone(float radius, float height, float massDensity,
			const vec3& initialPosition, const MathEngine::Quaternion& initialOrientation, const RenderingEngine::Color& color,
			const std::vector<ShapesEngine::Vertex>& vertices, const std::vector<ShapesEngine::Triangle>& triangles);

		/**@brief Returns the radius of the cone.
		*/
		float GetRadius() const;

		/**@brief Returns the height of the cone.
		*/
		float GetHeight() const;

		/**@brief Sets the radius of the cone.
		*/
		void SetRadius(float radius);

		/**@brief Sets the height of the cone.
		*/
		void SetHeight(float height);

		/**@brief Returns the RigidBody object of the RigidCone.
		*/
		const RigidBody& GetRigidBody() const;

		/**@brief Returns the RigidBody object of the RigidCone.
		*/
		RigidBody& GetRigidBody();

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ShapesEngine::ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape.
		*/
		ShapesEngine::ThreeDimensionalShape& GetShape();

		/**@brief Sets the position of the RigidCone.
		*/
		void SetPosition(const vec3& position);

		/**@brief Updates the model matrix of the RigidCone.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the cone.
		*/
		float Volume();

	private:
		float mRadius;
		float mHeight;

		RigidBody mRigidBody;
		ShapesEngine::ThreeDimensionalShape mShape;
		vec3 mOffset;
	};
}