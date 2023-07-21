#pragma once

/** @file FACone.h
*	@brief File has a Cone class under the namespace FAShapes.
*/

#include "FAThreeDimensionalShape.h"

namespace FAShapes
{
	/** @class Cone ""
	*	@brief This is class is used to create a cone.
	*/
	class Cone
	{
	public:

		/**@brief Creates a Cone object.
		* Call InitializeCone to initialize the cone.
		*/
		Cone();

		/**@brief Initializes the properties of the cone.
		*
		* @param[in] width The radius of the cone.
		* @param[in] height The height of the cone.
		* @param[in] position The position of the cone.
		* @param[in] orientation The orientation of the cone.
		* @param[in] color The color of the cone.
		*/
		void InitializeCone(float radius, float height, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
			const FAColor::Color& color);

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		ThreeDimensionalShape& GetShape();

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

		/**@brief Updates the cones model matrix.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the cone.
		*/
		float Volume();

	private:
		FAShapes::ThreeDimensionalShape mShape;

		float mRadius;
		float mHeight;
	};
}