#pragma once

/** @file FACylinder.h
*	@brief File has a Cylinder class under the namespace FAShapes.
*/

#include "FAThreeDimensionalShape.h"

namespace FAShapes
{
	/** @class Cylinder ""
	*	@brief This is class is used to create a cylinder.
	*/
	class Cylinder
	{
	public:

		/**@brief Creates a Cylinder object.
		* Call InitializeCylinder to initialize the cylinder.
		*/
		Cylinder();

		/**@brief Initializes the properties of the cylinder.
		*
		* @param[in] width The radius of the cylinder.
		* @param[in] height The height of the cylinder.
		* @param[in] position The position of the cylinder.
		* @param[in] orientation The orientation of the cylinder.
		* @param[in] color The color of the cylinder.
		*/
		void InitializeCylinder(float radius, float height, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
			const FAColor::Color& color);

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		ThreeDimensionalShape& GetShape();

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

		/**@brief Updates the cylinders model matrix.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the cylinder.
		*/
		float Volume();

	private:
		FAShapes::ThreeDimensionalShape mShape;

		float mRadius;
		float mHeight;
	};
}