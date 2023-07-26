#pragma once

#include "RigidBody.h"
#include "ThreeDimensionalShape.h"

namespace PhysicsEngine
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
			const vec3& initialPosition, const MathEngine::Quaternion& initialOrientation, const RenderingEngine::Color& color,
			const std::vector<ShapesEngine::Vertex>& vertices, const std::vector<ShapesEngine::Triangle>& triangles);

		/**@brief Returns the radius of the sphere.
		*/
		float GetRadius() const;
		/**@brief Sets the radius of the sphere.
		*/
		void SetRadius(float radius);

		/**@brief Returns the RigidBody object.
		*/
		const RigidBody& GetRigidBody() const;

		/**@brief Returns the RigidBody object.
		*/
		RigidBody& GetRigidBody();

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ShapesEngine::ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		ShapesEngine::ThreeDimensionalShape& GetShape();

		/**@brief Sets the position of the RigidSphere.
		*/
		void SetPosition(const vec3& position);

		/**@brief Updates the model matrix of the RigidSphere.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the sphere.
		*/
		float Volume();

	private:
		float mRadius;

		RigidBody mRigidBody;
		ShapesEngine::ThreeDimensionalShape mShape;
		vec3 mOffset;
	};
}