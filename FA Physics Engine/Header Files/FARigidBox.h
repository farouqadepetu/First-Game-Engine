#pragma once

#include "FARigidBody.h"
#include "FAThreeDimensionalShape.h"

namespace FAPhysicsShapes
{
	class RigidBox

	{
	public:

		/**@brief Default Constructor.
		* Constructs a RigidBox object.
		*/
		RigidBox();

		/**@brief Iniitalizes a rigid box that can be used to do physics simulations.
		*
		* @param[in] width The width of the box.
		* @param[in] height The height of the box.
		* @param[in] depth The depth of the box.
		* @param[in] color The color of the box.
		* @param[in] massDensity The mass density of the box.
		* @param[in] initialPosition The initial position of the box.
		* @param[in] initialOrientation The initial orientation of the box.
		* @param[in] color The color of the box.
		* @param[in] vertices The vertex list of a unit box.
		* @param[in] triangles The triangle list of a unit box.
		*/
		void InitializeRigidBox(float width, float height, float depth, float massDensity,
			const FAMath::Vector4D& initialPosition, const FAMath::Quaternion& initialOrientation, const FAColor::Color& color,
			const std::vector<FAShapes::Vertex>& vertices, const std::vector<FAShapes::Triangle>& triangles);

		/**@brief Returns the width of the box.
		*/
		float GetWidth() const;

		/**@brief Returns the height of the box.
		*/
		float GetHeight() const;

		/**@brief Returns the depth of the box.
		*/
		float GetDepth() const;

		/**@brief Sets the width of the box.
		*/
		void SetWidth(float width);

		/**@brief Sets the height of the box.
		*/
		void SetHeight(float height);

		/**@brief Sets the depth of the box.
		*/
		void SetDepth(float depth);

		/**@brief Returns the RigidBody object.
		*/
		const FAPhysics::RigidBody& GetRigidBody() const;

		/**@brief Returns the RigidBody object,
		*/
		FAPhysics::RigidBody& GetRigidBody();

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const FAShapes::ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		FAShapes::ThreeDimensionalShape& GetShape();

		/**@brief Sets the position of the RigidBox.
		*/
		void SetPosition(const FAMath::Vector4D& position);

		/**@brief Updates the model matrix of the RigidBox.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the box.
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