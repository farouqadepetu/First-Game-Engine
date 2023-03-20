#pragma once

/** @file FABox.h
*	@brief File has a Box class under the namespace FAShapes.
*/

//#include "FAVector3D.h"
//#include "FAMatrix4x4.h"
//#include "FAQuaternion.h"
//#include "FAShapesUtility.h"
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
		static const unsigned int NUMOFVERTICES{ 8 };

		/**@brief Number of triangles that make up the box.
		*/
		static const unsigned int NUMOFTRIANGLES{ 12 };

		/**@brief Constructor.
		* Creates a Box of length 2 centered around the origin.
		* In a left-handed coordinate system the front face looks towards +z axis, the top face looks towards the +y axis and the
		* right face looks towards the +x axis. /n
		* The Box is made using triangles. The vertices are ordered in clockwise order.
		*/
		Box(FAColor::Color color = FAColor::Color(0.0f, 0.0f, 0.0f, 1.0f));

		/**@brief Returns the location of the center of the box.
		*/
		FAMath::Vector3D getCenter() const;

		/**@brief Returns the x axis of the local coordinate system.
		*/
		FAMath::Vector3D getXAxis() const;

		/**@brief Returns the y axis of the local coordinate system.
		*/
		FAMath::Vector3D getYAxis() const;

		/**@brief Returns the z axis of the local coordinate system.
		*/
		FAMath::Vector3D getZAxis() const;

		/**@brief Returns a pointer to the vertex list of the box.
		*/
		Vertex* getVertexList();

		/**@brief Returns a pointer to the triangle list of the box.
		*/
		Triangle* getTriangleList();

		/**@brief Returns a pointer to a const vertex list of the box.
		*/
		const Vertex* getVertexList() const;

		/**@brief Returns a pointer to a const triangle list of the box.
		*/
		const Triangle* getTriangleList() const;

		/**@brief Returns a constant reference to the triangle at the specified index.
		* Throws an out_of_range exception if index >= NUMOFTRIANGLES.
		*/
		const Triangle& getTriangle(unsigned int index) const;

		/**@brief Returns the color of the box.
		*/
		FAColor::Color getColor() const;

		/**@brief Returns the scale matrix of the box which is used to set the size of the box.
		*/
		FAMath::Matrix4x4 getScaleMatrix() const;

		/**@brief Returns the local to world transformation matrix of the box.
		*/
		FAMath::Matrix4x4 getLocalToWorldMatrix() const;

		/**@brief Returns the draw arguments of the box.
		*/
		DrawArguments getDrawArguments() const;

		/**@brief Sets the color of the box to the specified color.
		*/
		void setColor(const FAColor::Color& color);

		/**@brief Sets the color of the box to the specified RBBA values.
		*/
		void setColor(float r, float g, float b, float a);

		/**@brief Sets the color of the box to the specified matrix.
		*/
		void setScaleMatrix(const FAMath::Matrix4x4& scale);

		/**@brief Sets the center of the box to the specified center.
		*/
		void setCenter(const FAMath::Vector3D& center);

		/**@brief Sets the world rotation quaternion to the specified quaternion.
		*/
		void setWorldRotation(const FAMath::Quaternion& rotQuaternion);

		/**@brief Sets the world rotation quaternion to the rotation quaternion with the specified angle and axis.
		*/
		void setWorldRotation(float angle, const FAMath::Vector3D axis);

		/**@brief Sets the draw arguments of the box to the specifed draw arguments.
		*/
		void setDrawArguments(const DrawArguments& boxDrawArgs);

		/**@brief Sets the draw arguments of the box to the specifed draw arguments.
		*/
		void setDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex, 
			int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Rotates the local axis of the box by the specified rotation matrix.
		*/
		void rotateAxes(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the local axis of the box by the specified rotation quaternion.
		*/
		void rotateAxes(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the local axis of the box by the specified angle around the specified axis.
		* Uses a quaternion to rotate.
		*/
		void rotateAxes(float angle, const FAMath::Vector3D axis);

		/**@brief Rotates the center of the box by the specified rotation matrix.
		*/
		void rotateCenter(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the center of the box by the specified rotation quaternion.
		*/
		void rotateCenter(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the center of the box by the specified angle around the specified axis.
		* Uses a quaternion to rotate.
		*/
		void rotateCenter(float angle, const FAMath::Vector3D axis);

		/**@brief Translates the center by the specified values.
		*/
		void translateCenter(float x, float y, float z);

		/**@brief Translates the center by the specified vector.
		*/
		void translateCenter(FAMath::Vector3D v);

		/**@brief Updates the boxs' local to world transformation matrix.
		*/
		void updateLocalToWorldMatrix();

	private:

		//reference point
		FAMath::Vector3D mCenter;

		//local axis
		FAMath::Vector3D mX;
		FAMath::Vector3D mY;
		FAMath::Vector3D mZ;

		//local cooridnates
		Vertex mLocalVertices[NUMOFVERTICES];

		//the triangles that make up the box
		Triangle mTriangles[NUMOFTRIANGLES];

		//color of the box
		FAColor::Color mColor;

		//scale matrix
		FAMath::Matrix4x4 mScale;

		//local to world transformation matrix
		FAMath::Matrix4x4 mLocalToWorld;

		//the draw arguments used to draw the box
		DrawArguments mBoxDrawArguments;

		//strores the local vertices of box
		void createVertices();

		//creates the triangles from the local vertices
		void createTriangles();

		//creates the normals from the triangles
		void createNormals();

		//stores the indices of the vertices that make up each triangle.
		void quad(unsigned int& index, unsigned int a, unsigned int b, unsigned int c, unsigned int d);
	};
}