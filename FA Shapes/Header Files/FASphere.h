#pragma once

/** @file FASphere.h
*	@brief File has a Sphere class under the namespace FAShapes.
*/

#include "FAThreeDimensional.h"

namespace FAShapes
{
	/** @class Sphere ""
	*	@brief This is class is used to create a sphere.
	*/
	class Sphere : public ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Creates a sphere with the specified radius and color and it is centered around the origin.
		* 
		* Uses the UV method to create the sphere.\n
		* The more circles and vertices per cirlce, the more circular the sphere looks.
		* 
		* @param[in] radius The radius of the cone.
		* @param[in] color The color of the cone.
		* @param[in] numCircles The number of circles the cone has.
		* @param[in] numVerticesPerCircle The number of vertices each circle has.
		*/
		Sphere(float radius = 1.0f, const FAColor::Color& color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f),
			unsigned int numCircles = 20, unsigned int numVerticesPerCircle = 20);

		/**@brief Returns the radius of the sphere.
		*/
		float GetRadius() const;
		
		/**@brief Set the radius of the sphere to the specified value.
		*/
		void SetRadius(float r);

		/**@brief Updates the spheres local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix() override final;

		/**@brief Returns the volume of the sphere.
		*/
		float Volume() override final;

	private:
		//Radius of the sphere.
		float mRadius;

		//The number of slices the sphere has.
		unsigned int mNumCircles;

		//The number of vertices each slice has.
		unsigned int mNumVerticesPerCircle;

		//Creates the vertices of the sphere.
		void CreateVertices() override final;

		//Creates the triangles that make up the sphere.
		void CreateTriangles() override final;

		//Creates the normals of the sphere.
		void CreateNormals() override final;
	};
}