#pragma once

/** @file FABox.h
*	@brief File has a Box class under the namespace FAShapes.
*/

#include "FATriangle.h"


namespace FAShapes
{
	/** @class Box ""
	*	@brief This is class is used to draw a box.
	*/
	class Box
	{
	public:

		/**@brief Number of vertices the box has.
		*/
		static const unsigned int NUM_OF_VERTICES{ 8 };

		/**@brief Number of triangles that make up the box.
		*/
		static const unsigned int NUM_OF_TRIANGLES{ 12 };

		/**@brief Default Constructor.
		* Creates a Box of with width = height = depth = 1 centered around the origin.\n
		* In a left-handed coordinate system the front face looks towards +z axis, the top face looks towards the +y axis and the
		* right face looks towards the +x axis. /n
		* The Box is made using triangles. The vertices are ordered in clockwise order.
		* The default color is black.
		*/
		Box();

		/**@brief Overloaded Constructor.
		* Creates a Box of the specified width, height, depth and color centered around the origin.
		* In a left-handed coordinate system the front face looks towards +z axis, the top face looks towards the +y axis and the
		* right face looks towards the +x axis. /n
		* The Box is made using triangles. The vertices are ordered in clockwise order.
		*/
		Box(unsigned int width, unsigned int height, unsigned int depth, 
			FAColor::Color color);

		/**@brief Returns a pointer to the vertex list of the box.
		*/
		Vertex* GetVertexList();

		/**@brief Returns a pointer to the triangle list of the box.
		*/
		Triangle* GetTriangleList();

		/**@brief Returns a pointer to a const vertex list of the box.
		*/
		const Vertex* GetVertexList() const;

		/**@brief Returns a pointer to a const triangle list of the box.
		*/
		const Triangle* GetTriangleList() const;

		/**@brief Returns a constant reference to the triangle at the specified index.
		* Throws an out_of_range exception if index >= NUMOFTRIANGLES.
		*/
		const Triangle& GetTriangle(unsigned int index) const;

		/**@brief Returns the location of the center of the box.
		*/
		const FAMath::Vector3D& GetCenter() const;

		/**@brief Returns the x axis of the local coordinate system.
		*/
		const FAMath::Vector3D& GetXAxis() const;

		/**@brief Returns the y axis of the local coordinate system.
		*/
		const FAMath::Vector3D& GetYAxis() const;

		/**@brief Returns the z axis of the local coordinate system.
		*/
		const FAMath::Vector3D& GetZAxis() const;

		/**@brief Returns the width of the box.
		*/
		unsigned int GetWidth() const;

		/**@brief Returns the height of the box.
		*/
		unsigned int GetHeight() const;

		/**@brief Returns the depth of the box.
		*/
		unsigned int GetDepth() const;

		/**@brief Returns the color of the box.
		*/
		const FAColor::Color& GetColor() const;

		/**@brief Returns the local to world transformation matrix of the box.
		*/
		const FAMath::Matrix4x4& GetLocalToWorldMatrix() const;

		/**@brief Returns the draw arguments of the box.
		*/
		const DrawArguments& GetDrawArguments() const;

		/**@brief Sets the center of the box to the specified center.
		*/
		void SetCenter(const FAMath::Vector3D& center);

		/**@brief Sets the width of the box to the specified width.
		*/
		void SetWidth(unsigned int width);

		/**@brief Sets the height of the box to the specified height.
		*/
		void SetHeight(unsigned int height);

		/**@brief Sets the depth of the box to the specified depth.
		*/
		void SetDepth(unsigned int depth);

		/**@brief Sets the color of the box to the specified color.
		*/
		void SetColor(const FAColor::Color& color);

		/**@brief Sets the color of the box to the specified RBBA values.
		*/
		void SetColor(float r, float g, float b, float a);

		/**@brief Sets the draw arguments of the box to the specifed draw arguments.
		*/
		void SetDrawArguments(const DrawArguments& boxDrawArgs);

		/**@brief Sets the draw arguments of the box to the specifed draw arguments.
		*/
		void SetDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex, 
			int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Rotates the local axis of the box by the specified rotation matrix.
		*/
		void RotateAxes(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the local axis of the box by the specified rotation quaternion.
		*/
		void RotateAxes(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the local axis of the box by the specified angle around the specified axis.
		* Uses a quaternion to rotate.
		*/
		void RotateAxes(float angle, const FAMath::Vector3D axis);

		/**@brief Rotates the center of the box by the specified rotation matrix.
		*/
		void RotateCenter(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the center of the box by the specified rotation quaternion.
		*/
		void RotateCenter(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the center of the box by the specified angle around the specified axis.
		* Uses a quaternion to rotate.
		*/
		void RotateCenter(float angle, const FAMath::Vector3D axis);

		/**@brief Translates the center by the specified values.
		*/
		void TranslateCenter(float x, float y, float z);

		/**@brief Translates the center by the specified vector.
		*/
		void TranslateCenter(FAMath::Vector3D v);

		/**@brief Updates the boxs' local to world transformation matrix.
		*/
		void UpdateLocalToWorldMatrix();

	private:

		//local cooridnates
		Vertex mLocalVertices[NUM_OF_VERTICES];

		//the triangles that make up the box
		Triangle mTriangles[NUM_OF_TRIANGLES];

		//reference point
		FAMath::Vector3D mCenter;

		//local axis
		FAMath::Vector3D mX;
		FAMath::Vector3D mY;
		FAMath::Vector3D mZ;

		//Dimensions of the box
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned int mDepth;

		//if true update local to world matrix
		bool mUpdateModelMatrix;

		//color of the box
		FAColor::Color mColor;

		//local to world transformation matrix
		FAMath::Matrix4x4 mLocalToWorld;

		//the draw arguments used to draw the box
		DrawArguments mBoxDrawArguments{};

		//strores the local vertices of box
		void mCreateVertices();

		//creates the triangles from the local vertices
		void mCreateTriangles();

		//creates the normals from the triangles
		void mCreateNormals();

		//stores the indices of the vertices that make up each triangle.
		void mQuad(unsigned int& index, unsigned int a, unsigned int b, unsigned int c, unsigned int d);
	};
}