#pragma once

#include "FAVector3D.h"

/** @file FATriangle.h
*	@brief File has a Triangle class under the namespace FAShape.
*/

namespace FAShape
{
	/** @class Triangle ""
	*	@brief The class stores a pointer to a vertex list and indices to the vertices of the triangle.
	*/
	class Triangle
	{
	public:

		/**@brief Constructor.
		* If the 3 vertices that make up the triangle are colinear, a runtime_exception is thrown.
		*/
		Triangle(FAMath::Vector3D* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index);

		/**@brief Returns the P0 vertex of the triangle.
		*/
		FAMath::Vector3D getP0();

		/**@brief Returns the P1 vertex of the triangle.
		*/
		FAMath::Vector3D getP1();

		/**@brief Returns the P2 vertex of the triangle.
		*/
		FAMath::Vector3D getP2();

		/**@brief Returns the index of where P0 is in the vertex list.
		*/
		unsigned int getP0Index();

		/**@brief Returns the index of where P1 is in the vertex list.
		*/
		unsigned int getP1Index();

		/**@brief Returns the index of where P2 is in the vertex list.
		*/
		unsigned int getP2Index();

		/**@brief Returns the normal of the triangle.
		*/
		FAMath::Vector3D getNormal();

		/**@brief Returns the center of the triangle.
		*/
		FAMath::Vector3D getCenter();

		/**@brief Sets the pointer to a vertex list to the specified pointers.
		*/
		void setVertexList(FAMath::Vector3D* vertexList);

		/**@brief Sets the indices of the vertices that make up the triangle to the specified vertices.
		* If the 3 vertices that make up the triangle are colinear, a runtime_exception is thrown.
		*/
		void setTriangleIndices(unsigned int p0Index, unsigned int p1Index, unsigned int p2Index);

		/**@brief Sets the pointer to a vertex list to the specified pointers and
		*	sets the indices of the vertices that make up the triangle to the specified vertices.
		* If the 3 vertices that make up the triangle are colinear, a runtime_exception is thrown.
		*/
		void setTriangle(FAMath::Vector3D* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index);


	private:
		FAMath::Vector3D* mVertexList; //pointer to an vertex list
		unsigned int mIndexList[3]; //indices into a vertex list
		bool isColinear();
	};
}