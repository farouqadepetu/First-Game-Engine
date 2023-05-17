#pragma once

#include "FAShapesUtility.h"

/** @file FATriangle.h
*	@brief File has a Triangle class under the namespace FAShapes.
*/

namespace FAShapes
{
	/** @class Triangle ""
	*	@brief The class stores a pointer to a vertex list and indices to the vertices of the triangle.
	*/
	class Triangle
	{
	public:

		/**@brief Constructs a triangle.
		* 
		* @param[in] vertexList A pointer to a vertex list.
		* @param[in] p0Index The index of the first point of the triangle.
		* @param[in] p1Index The index of the second point of the triangle.
		* @param[in] p2Index The index of the third point of the triangle.
		*/
		Triangle(Vertex* vertexList = nullptr, unsigned int p0Index = 0, unsigned int p1Index = 0, unsigned int p2Index = 0);

		/**@brief Returns a constant reference to the P0 vertex of the triangle.
		*/
		const Vertex& GetP0() const;

		/**@brief Returns a constant reference to the P1 vertex of the triangle.
		*/
		const Vertex& GetP1() const;

		/**@brief Returns a constant reference to the P2 vertex of the triangle.
		*/
		const Vertex& GetP2() const;

		/**@brief Returns the index of where P0 is in the vertex list.
		*/
		unsigned int GetP0Index() const;

		/**@brief Returns the index of where P1 is in the vertex list.
		*/
		unsigned int GetP1Index() const;

		/**@brief Returns the index of where P2 is in the vertex list.
		*/
		unsigned int GetP2Index() const;

		/**@brief Returns the normal of the triangle.
		*/
		FAMath::Vector4D GetNormal() const;

		/**@brief Returns the center of the triangle.
		*/
		FAMath::Vector4D GetCenter() const;

		/**@brief Sets the pointer to a vertex list to the specified pointers.
		*/
		void SetVertexList(Vertex* vertexList);

		/**@brief Sets the P0 index to the specified \a index.
		*/
		void SetP0Index(unsigned int index);

		/**@brief Sets the P1 index to the specified \a index.
		*/
		void SetP1Index(unsigned int index);

		/**@brief Sets the P2 index to the specified \a index.
		*/
		void SetP2Index(unsigned int index);

		/**@brief Sets the indices of the vertices that make up the triangle to the specified vertices.
		* 
		* @param[in] p0Index The index of the first point of the triangle.
		* @param[in] p1Index The index of the second point of the triangle.
		* @param[in] p2Index The index of the third point of the triangle.
		*/
		void SetTriangleIndices(unsigned int p0Index, unsigned int p1Index, unsigned int p2Index);

		/**@brief Sets the triangle variables.
		*	
		* @param[in] vertexList A pointer to a vertex list.
		* @param[in] p0Index The index of the first point of the triangle.
		* @param[in] p1Index The index of the second point of the triangle.
		* @param[in] p2Index The index of the third point of the triangle.
		*/
		void SetTriangle(Vertex* vertexList, unsigned int p0Index, unsigned int p1Index, unsigned int p2Index);

	private:
		Vertex* mVertexList; //pointer to a vertex list
		unsigned int mIndexList[3]; //indices into a vertex list
	};
}