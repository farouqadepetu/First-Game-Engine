#pragma once

/** @file FASphere.h
*	@brief File has a Sphere class under the namespace FAShapes.
*/

#include "FAThreeDimensionalShape.h"

namespace FAShapes
{
	/** @class Sphere ""
	*	@brief This is class is used to create a sphere.
	*/
	class Sphere
	{
	public:

		/**@brief Creates a Sphere object.
		* Call InitializeSphere to initialize the sphere.
		*/
		Sphere();

		/**@brief Initializes the properties of the sphere.
		*
		* @param[in] width The radius of the sphere.
		* @param[in] position The position of the sphere.
		* @param[in] orientation The orientation of the sphere.
		* @param[in] color The color of the sphere.
		*/
		void InitializeSphere(float radius, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
			const FAColor::Color& color);

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		ThreeDimensionalShape& GetShape();

		/**@brief Returns the radius of the sphere.
		*/
		float GetRadius() const;

		/**@brief Sets the radius of the sphere.
		*/
		void SetRadius(float radius);

		/**@brief Updates the spheres model matrix.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the sphere.
		*/
		float Volume();

	private:
		FAShapes::ThreeDimensionalShape mShape;

		float mRadius;
	};
}