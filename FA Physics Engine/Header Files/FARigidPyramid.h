#pragma once

#include "FARigidBody.h"
#include "FAThreeDimensionalShape.h"

namespace FAPhysicsShapes
{
	class RigidPyramid

	{
	public:

		/**@brief Default Constructor.
		* Constructs a RigidPyramid object.
		*/
		RigidPyramid();

		/**@brief Iniitalizes a rigid pyramid that can be used to do physics simulations.
		*
		* @param[in] width The width of the pyramid.
		* @param[in] height The height of the pyramid.
		* @param[in] depth The depth of the pyramid.
		* @param[in] color The color of the pyramid.
		* @param[in] massDensity The mass density of the pyramid.
		* @param[in] initialPosition The initial position of the pyramid.
		* @param[in] initialOrientation The initial orientation of the pyramid.
		* @param[in] color The color of the pyramid.
		* @param[in] vertices The vertex list of a unit pyramid.
		* @param[in] triangles The triangle list of a unit pyramid.
		*/
		void InitializeRigidPyramid(float width, float height, float depth, float massDensity,
			const FAMath::Vector4D& initialPosition, const FAMath::Quaternion& initialOrientation, const FAColor::Color& color,
			const std::vector<FAShapes::Vertex>& vertices, const std::vector<FAShapes::Triangle>& triangles);

		/**@brief Returns the width of the pyramid.
		*/
		float GetWidth() const;

		/**@brief Returns the height of the pyramid.
		*/
		float GetHeight() const;

		/**@brief Returns the depth of the pyramid.
		*/
		float GetDepth() const;

		/**@brief Sets the width of the pyramid.
		*/
		void SetWidth(float width);

		/**@brief Sets the height of the pyramid.
		*/
		void SetHeight(float height);

		/**@brief Sets the depth of the pyramid.
		*/
		void SetDepth(float depth);

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

		/**@brief Sets the position of the RigidPyramid.
		*/
		void SetPosition(const FAMath::Vector4D& position);

		/**@brief Updates the model matrix of the RigidPyramid.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the pyramid.
		*/
		float Volume();

	private:
		float mWidth;
		float mHeight;
		float mDepth;

		FAPhysics::RigidBody mRigidBody;
		FAShapes::ThreeDimensionalShape mShape;
		FAMath::Vector4D mOffset;
	};
}