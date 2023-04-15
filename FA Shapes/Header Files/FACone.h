#pragma once

/** @file FACone.h
*	@brief File has a Cone class under the namespace FAShapes.
*/

#include "FAThreeDimensional.h"

namespace FAShapes
{
	/** @class Cone ""
	*	@brief This is class is used to create a cone.
	*/
	class Cone : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief  Creates a cone with the specified radius, height and color and it is centered around the origin.
		* 
		* Uses the UV method to create the cone.\n
		* The more circles and vertices per cirlce, the more circular the cone looks.
		* 
		* @param[in] radius The radius of the cone.
		* @param[in] height The height of the cone.
		* @param[in] color The color of the cone.
		* @param[in] fillBottom Pass in true to fill in the bottom of the cone.
		* @param[in] numCircles The number of circles the cone has.
		* @param[in] numVerticesPerCircle The number of vertices each circle has.
		*/
		Cone(float radius = 1.0f, float height = 1.0f, const FAColor::Color& color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f),
			bool fillBottom = false,
			unsigned int numCircles = 20, unsigned int numVerticesPerCircle = 20);

		/**@brief Returns the radius of the base of the cone.
		*/
		float GetRadius() const;

		/**@brief Returns the height of the base of the cone.
		*/
		float GetHeight() const;

		/**@brief Sets the radius of the base of the cone to the specified value.
		*/
		void SetRadius(float r);

		/**@brief Sets the height of the base of the cone to the specified value.
		*/
		void SetHeight(float h);

		/**@brief Updates the cones local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix() override final;

		/**@brief Returns the volume of the cone.
		*/
		float Volume() override final;

	private:

		//Radius of the cone.
		float mRadius;

		//Height of the cone
		float mHeight;

		//The number of slices the cone has.
		unsigned int mNumCircles;

		//The number of vertices each slice has.
		unsigned int mNumVerticesPerCircle;

		//True to fill the bottom, false to not fill the bottom.
		bool mFillBottom;

		//Creates the vertices of the cone.
		void CreateVertices() override final;

		//Creates the triangles that make up the cone.
		void CreateTriangles() override final;
	};
}