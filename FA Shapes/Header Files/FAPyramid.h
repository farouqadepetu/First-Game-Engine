#pragma once

/** @file FAPyramid.h
*	@brief File has a Pyramid class under the namespace FAShapes.
*/

#include "FAThreeDimensionalShape.h"

namespace FAShapes
{
	/** @class Pyramid ""
	*	@brief This is class is used to create a pyramid.
	*/
	class Pyramid
	{
	public:

		/**@brief Creates a Pyramid object.
		* Call InitializePyramid to initialize the pyramid.
		*/
		Pyramid();

		/**@brief Initializes the properties of the pyramid.
		*
		* @param[in] width The width of the pyramid.
		* @param[in] height The height of the pyramid.
		* @param[in] depth The depth of the pyramid.
		* @param[in] position The position of the pyramid.
		* @param[in] orientation The orientation of the pyramid.
		* @param[in] color The color of the pyramid.
		*/
		void InitializePyramid(float width, float height, float depth, const FAMath::Vector4D position, const FAMath::Quaternion orientation,
			const FAColor::Color& color);

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		ThreeDimensionalShape& GetShape();

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

		/**@brief Updates the pyramids model matrix.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the pyramid.
		*/
		float Volume();

	private:
		FAShapes::ThreeDimensionalShape mShape;

		float mWidth;
		float mHeight;
		float mDepth;
	};
}