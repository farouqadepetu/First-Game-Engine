#pragma once

/** @file FABox.h
*	@brief File has a Box class under the namespace FAShapes.
*/


#include "FAThreeDimensional.h"


namespace FAShapes
{
	/** @class Box ""
	*	@brief This is class is used to create a box.
	*/
	class Box : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a Box with the specified width, height, depth and color centered around the origin.
		* 
		* In a left-handed coordinate system the front face looks towards +z axis, the top face looks towards the +y axis and the
		* right face looks towards the +x axis./n
		* The Box is made using triangles. The vertices are ordered in clockwise order.
		* 
		* @param[in] width The width of the box.
		* @param[in] height The height of the box.
		* @param[in] depth The depth of the box.
		* @param[in] color The color of the box.
		*/
		Box(float width = 1.0f, float height = 1.0f , float depth = 1.0f, 
			const FAColor::Color& color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f));

		/**@brief Returns the width of the box.
		*/
		float GetWidth() const;

		/**@brief Returns the height of the box.
		*/
		float GetHeight() const;

		/**@brief Returns the depth of the box.
		*/
		float GetDepth() const;

		/**@brief Sets the width of the box to the specified width.
		*/
		void SetWidth(float width);

		/**@brief Sets the height of the box to the specified \a height.
		*/
		void SetHeight(float height);

		/**@brief Sets the depth of the box to the specified \a depth.
		*/
		void SetDepth(float depth);

		/**@brief Updates the boxs local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix() override final;

		/**@brief Returns the volume of the box.
		*/
		float Volume() override final;

	private:
		//Dimensions of the box
		float mWidth;
		float mHeight;
		float mDepth;

		//Creates the vertices of the box.
		void CreateVertices() override final;

		//Creates the triangles that make up box.
		void CreateTriangles() override final;
	};
}