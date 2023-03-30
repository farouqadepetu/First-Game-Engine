#pragma once

#include "FATriangle.h"
#include <vector>

namespace FAShapes
{
	/** @struct ThreeDimensionalShapeAbstract ""
	*	@brief An abstract class for 3D shapes.
	*/
	class ThreeDimensionalShapeAbstract
	{
	public:

		/**@brief Constructor.
		*/
		ThreeDimensionalShapeAbstract(const FAColor::Color& color);

		/**@brief Returns a constant reference to the center of the 3D shape.
		*/
		const FAMath::Vector3D& GetCenter() const;

		/**@brief Returns a constant reference to the x axis of the 3D shape.
		*/
		const FAMath::Vector3D& GetXAxis() const;

		/**@brief Returns a constant reference to the y axis of the 3D shape.
		*/
		const FAMath::Vector3D& GetYAxis() const;

		/**@brief Returns a constant reference to the z axis of the 3D shape.
		*/
		const FAMath::Vector3D& GetZAxis() const;

		/**@brief Returns a constant reference to the local to world matrix of the 3D shape.
		*/
		const FAMath::Matrix4x4& GetLocalToWorldMatrix() const;

		/**@brief Returns a constant pointer to the local vertices of the 3D shape.
		*/
		const Vertex* GetLocalVertices() const;

		/**@brief Returns a constant pointer to the triangles of the 3D shape.
		*/
		const Triangle* GetTriangleList() const;

		/**@brief Returns a constant reference to the specified triangle.
		*/
		const Triangle& GetTriangle(unsigned int index) const;

		/**@brief Returns a constant reference to the draw arguments of the 3D shape.
		*/
		const DrawArguments& GetDrawArguments() const;

		/**@brief Returns a constant reference to the color of the 3D shape.
		*/
		const FAColor::Color& GetColor() const;

		/**@brief Returns the number of triangles the 3D shape has.
		*/
		size_t GetNumTriangles() const;

		/**@brief Returns the number of vertices the 3D shape has.
		*/
		size_t GetNumVertices() const;

		/**@brief Sets the center of the 3D shape to the specified vector.
		*/
		void SetCenter(const FAMath::Vector3D& center);

		/**@brief Sets the center of the 3D shape to the specified values.
		*/
		void SetCenter(float x, float y, float z);

		/**@brief Sets the color of the sphere to the specified color.
		*/
		void SetColor(const FAColor::Color& color);

		/**@brief Sets the color of the sphere to the specified RBBA values.
		*/
		void SetColor(float r, float g, float b, float a);

		/**@brief Sets the draw arguments of the sphere to the specifed draw arguments.
		*/
		void SetDrawArguments(const DrawArguments& sphereDrawArgs);

		/**@brief Sets the draw arguments of the sphere to the specifed draw arguments.
		*/
		void SetDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex,
			int indexOfFirstVertex, int indexOfConstantData);

		/**@brief Rotates the local axis of the sphere by the specified rotation matrix.
		*/
		void RotateAxes(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the local axis of the sphere by the specified rotation quaternion.
		*/
		void RotateAxes(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the local axis of the sphere by the specified angle around the specified axis.
		* Uses a quaternion to rotate.
		*/
		void RotateAxes(float angle, const FAMath::Vector3D axis);

		/**@brief Rotates the center of the sphere by the specified rotation matrix.
		*/
		void RotateCenter(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the center of the sphere by the specified rotation quaternion.
		*/
		void RotateCenter(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the center of the sphere by the specified angle around the specified axis.
		* Uses a quaternion to rotate.
		*/
		void RotateCenter(float angle, const FAMath::Vector3D axis);

		/**@brief Translates the center by the specified values.
		*/
		void TranslateCenter(float x, float y, float z);

		/**@brief Translates the center by the specified vector.
		*/
		void TranslateCenter(const FAMath::Vector3D& v);

	
		virtual void UpdateLocalToWorldMatrix() = 0;

	protected:
		//Center of the 3D shape.
		FAMath::Vector3D mCenter;

		//Local axes of the 3D shape.
		FAMath::Vector3D mX;
		FAMath::Vector3D mY;
		FAMath::Vector3D mZ;

		//Color of the 3D shape.
		FAColor::Color mColor;

		//If true, updates the local to world matrix.
		bool mUpdateModelMatrix;

		//Local to world matrix of the 3D shape.
		FAMath::Matrix4x4 mLocalToWorld;

		//Local vertices of the 3D shape.
		std::vector<Vertex> mLocalVertices;

		//The triangles that make up the 3D shape.
		std::vector<Triangle> mTriangles;

		//The arguments needed to render the 3D shape.
		DrawArguments mSphereDrawArguments{};

		/**@brief Stores the indices of the vertices of the triangles that make up the 3D shape.
		*/
		void Quad(unsigned int a, unsigned int b, unsigned int c, unsigned int d);

		/**@brief Creates the local vertices of the 3D shape.
		*/
		virtual void CreateVertices() = 0;

		/**@brief Creates the triangles that make up the 3D shape.
		*/
		virtual void CreateTriangles() = 0;

		/**@brief Creates the normals of each vertex.
		*/
		virtual void CreateNormals() = 0;

	};
}