#pragma once

/** @file FACylinder.h
*	@brief File has a Cylinder class under the namespace FAShapes.
*/

#include "FAThreeDimensional.h"

namespace FAShapes
{
	class Cylinder : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a cylinder with the specified radius, height and color and it is centered around the origin.
		* 
		* Uses the UV method to create the cylinder.\n
		* The more circles and vertices per cirlce, the more circular it looks.
		* 
		* @param[in] radius The radius of the cylinder.
		* @param[in] height The height of the cylinder.
		* @param[in] color The color of the cylinder.
		* @param[in] fillTopAndBottom Pass in true to fill in the top and bottom of the cylinder.
		* @param[in] numCircles The number of circles the cylinder has.
		* @param[in] numVerticesPerCircle The number of vertices each circle has.
		*/
		Cylinder(float radius = 1.0f, float height = 1.0f, const FAColor::Color& color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f),
			bool fillTopAndBottom = false,
			unsigned int numCircles = 20, unsigned int numVerticesPerCircle = 20);

		/**@brief Returns the radius of the cylinder.
		*/
		float GetRadius() const;

		/**@brief Returns the height of the cylinder.
		*/
		float GetHeight() const;

		/**@brief Sets the radius of the cylinder to the specified value.
		*/
		void SetRadius(float r);

		/**@brief Sets the height of the cylinder to the specified value.
		*/
		void SetHeight(float h);

		/**@brief Updates the cylinders local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix() override final;

		/**@brief Returns the volume of the cylinder.
		*/
		float Volume() override final;

	private:

		//radius of the cylinder
		float mRadius;

		//Height of the cylinder
		float mHeight;

		//The number of slices the cylinder has.
		unsigned int mNumCircles;

		//The number of vertices each slice has.
		unsigned int mNumVerticesPerCircle;

		//True to fill the top and bottom, false to not fill the top and bottom.
		bool mFillTopAndBottom;

		//Creates the vertices of the cylinder.
		void CreateVertices() override final;

		//Creates the triangles that make up the cylinder.
		void CreateTriangles() override final;

		//Creates the normals of the cylinder.
		void CreateNormals() override final;

	};
}