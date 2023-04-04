#pragma once

/** @file FAPyramid.h
*	@brief File has a Pyramid class under the namespace FAShapes.
*/

#include "FAThreeDimensional.h"

namespace FAShapes
{
	/** @class Pyramid ""
	*	@brief This is class is used to create a pyramid.
	*/
	class Pyramid : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a pyramid with the specified width, height, depth and color centered around the origin.
		* 
		* In a left-handed coordinate system the front of the pyramid looks towards +z axis, 
		* the base of the pyramid looks towards the -y axis and the
		* right face looks towards the +x axis. /n
		* The vertices are ordered in clockwise order.
		* 
		* @param[in] width The width of the pyramid.
		* @param[in] height The height of the pyramid.
		* @param[in] depth The depth of the pyramid.
		* @param[in] color The color of the pyramid.
		*/
		Pyramid(float width = 1.0f, float height = 1.0f, float depth = 1.0f,
			const FAColor::Color& color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f));

		/**@brief Returns the width of the pyramid.
		*/
		float GetWidth() const;

		/**@brief Returns the height of the pyramid.
		*/
		float GetHeight() const;

		/**@brief Returns the depth of the pyramid.
		*/
		float GetDepth() const;

		/**@brief Sets the width of the pyramid to the specified \a width.
		*/
		void SetWidth(float width);

		/**@brief Sets the height of the pyramid to the specified \a height.
		*/
		void SetHeight(float height);

		/**@brief Sets the depth of the pyramid to the specified \a depth.
		*/
		void SetDepth(float depth);

		/**@brief Updates the pyramids local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix() override final;

		/**@brief Returns the volume of the pyramid.
		*/
		float Volume() override final;

	private:
		//Dimensions of the pyramid
		float mWidth;
		float mHeight;
		float mDepth;

		//Creates the vertices of the pyramid.
		void CreateVertices() override final;

		//Creates the triangles that make up pyramid.
		void CreateTriangles() override final;

		//Creates the normals of the pyramid.
		void CreateNormals() override final;
	};
}