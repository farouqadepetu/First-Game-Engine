#pragma once

/** @file FABox.h
*	@brief File has a Box class under the namespace FAShapes.
*/


#include "FAThreeDimensional.h"


namespace FAShapes
{
	/** @class Box ""
	*	@brief This is class is used to render a box and for bounding objects.
	*/
	class Box : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Constructor.
		* Creates a Box with the specified width, height, depth and color centered around the origin.
		* In a left-handed coordinate system the front face looks towards +z axis, the top face looks towards the +y axis and the
		* right face looks towards the +x axis. /n
		* The Box is made using triangles. The vertices are ordered in clockwise order.
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

		/**@brief Sets the height of the box to the specified height.
		*/
		void SetHeight(float height);

		/**@brief Sets the depth of the box to the specified depth.
		*/
		void SetDepth(float depth);

		/**@brief Updates the boxs local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix() override final;

	private:
		//Dimensions of the box
		float mWidth;
		float mHeight;
		float mDepth;

		//Creates the vertices of the box.
		void CreateVertices() override final;

		//Creates the triangles that make up box.
		void CreateTriangles() override final;

		//Creates the normals of the box.
		void CreateNormals() override final;
	};
}