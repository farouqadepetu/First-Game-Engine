#pragma once


#include "ThreeDimensionalShape.h"

namespace ShapesEngine
{
	/** @class Box ""
	*	@brief This class is used to create a box.
	*/
	class Box
	{
	public:

		/**@brief Creates a Box object.
		* Call InitializeBox to initialize the box.
		*/
		Box();

		/**@brief Initializes the properties of the box.
		*
		* @param[in] width The width of the box.
		* @param[in] height The height of the box.
		* @param[in] depth The depth of the box.
		* @param[in] position The position of the box.
		* @param[in] orientation The orientation of the box.
		* @param[in] color The color of the box.
		*/
		void InitializeBox(float width, float height, float depth, const vec3 position, const MathEngine::Quaternion orientation,
			const RenderingEngine::Color& color);

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		const ThreeDimensionalShape& GetShape() const;

		/**@brief Returns the ThreeDimensionalShape object.
		*/
		ThreeDimensionalShape& GetShape();

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

		/**@brief Updates the boxs model matrix.
		*/
		void UpdateModelMatrix();

		/**@brief Returns the volume of the box.
		*/
		float Volume();

	private:
		ThreeDimensionalShape mShape;

		float mWidth;
		float mHeight;
		float mDepth;
	};
}