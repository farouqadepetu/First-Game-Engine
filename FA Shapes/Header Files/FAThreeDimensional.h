#pragma once

/** @file FAThreeDimensional.h
*	@brief File has the abstract class ThreeDimensionalShapeAbstract under the namespace FAShapes.
*/

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

		/**@brief Constructs a 3D shape.
		* 
		* @param[in] color The color if the 3D shape.
		*/
		ThreeDimensionalShapeAbstract(const FAColor::Color& color);

		/**@brief Returns a constant reference to the center of the 3D shape.
		*/
		const FAMath::Vector4D& GetCenter() const;

		/**@brief Returns a constant reference to the x axis of the 3D shape.
		*/
		const FAMath::Vector4D& GetXAxis() const;

		/**@brief Returns a constant reference to the y axis of the 3D shape.
		*/
		const FAMath::Vector4D& GetYAxis() const;

		/**@brief Returns a constant reference to the z axis of the 3D shape.
		*/
		const FAMath::Vector4D& GetZAxis() const;

		/**@brief Returns a constant reference to the local to world matrix of the 3D shape.
		*/
		const FAMath::Matrix4x4& GetLocalToWorldMatrix() const;

		/**@brief Returns a constant pointer to the local vertices of the 3D shape.
		*/
		const Vertex* GetLocalVertices() const;

		/**@brief Returns a constant pointer to the triangles of the 3D shape.
		*/
		const Triangle* GetTriangleList() const;

		/**@brief Returns a pointer to the local vertices of the 3D shape.
		*/
		Vertex* GetLocalVertices();

		/**@brief Returns a pointer to the triangles of the 3D shape.
		*/
		Triangle* GetTriangleList();

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

		/**@brief Sets the center of the 3D shape to the specified vector \a center.
		*/
		void SetCenter(const FAMath::Vector4D& center);

		/**@brief Sets the center of the 3D shape to the specified values.
		*/
		void SetCenter(float x, float y, float z);

		/**@brief Sets the local x-axis of the 3D shape to the specified values.
		*/
		void SetXAxis(float x, float y, float z);

		/**@brief Sets the local y-axis of the 3D shape to the specified values.
		*/
		void SetYAxis(float x, float y, float z);

		/**@brief Sets the local z-axis of the 3D shape to the specified values.
		*/
		void SetZAxis(float x, float y, float z);

		/**@brief Sets the color of the sphere to the specified \a color.
		*/
		void SetColor(const FAColor::Color& color);

		/**@brief Sets the color of the 3D shape to the specified RGBA values.
		*/
		void SetColor(float r, float g, float b, float a);

		/**@brief Sets the draw arguments of the 3D shape to the specifed draw arguments \a sphereDrawArgs.
		*/
		void SetDrawArguments(const DrawArguments& drawArgs);

		/**@brief Sets the draw arguments of the 3D shape to the specifed draw arguments.
		*/
		void SetDrawArguments(unsigned int indexCount, unsigned int locationOfFirstIndex,
			int indexOfFirstVertex, unsigned int indexOfConstantData);

		/**@brief Rotates the local axis of the 3D shape by the specified rotation matrix \a rot.
		*/
		void RotateAxes(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the local axis of the 3D shape by the specified rotation quaternion \a rotQuaternion.
		*/
		void RotateAxes(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the local axis of the 3D shape by the specified \a angle around the specified \a axis.
		* 
		* Uses a quaternion to rotate.
		*/
		void RotateAxes(float angle, const FAMath::Vector3D& axis);

		/**@brief Rotates the center of the 3D shape by the specified rotation matrix \a rot.
		*/
		void RotateCenter(const FAMath::Matrix4x4& rot);

		/**@brief Rotates the center of the 3D shape by the specified rotation quaternion \a rotQuaternion.
		*/
		void RotateCenter(const FAMath::Quaternion& rotQuaternion);

		/**@brief Rotates the center of the 3D shape by the specified \a angle around the specified \a axis.
		* 
		* Uses a quaternion to rotate.
		*/
		void RotateCenter(float angle, const FAMath::Vector3D& axis);

		/**@brief Translates the center by the specified values.
		*/
		void TranslateCenter(float x, float y, float z);

		/**@brief Translates the center by the specified vector \a v.
		*/
		void TranslateCenter(const FAMath::Vector3D& v);

		/**@brief Updates the local to world matrix for the 3D shape.
		*/
		virtual void UpdateLocalToWorldMatrix() = 0;

		/**@brief Returns the volume of the 3D shape.
		*/
		virtual float Volume() = 0;

#if defined(_DEBUG)
		inline void PrintVertices()
		{
			int j = 0;
			for (auto& i : mLocalVertices)
			{
				auto worldPos = i.position * mLocalToWorld;
				auto worldNormal = i.normal * Transpose(Inverse(mLocalToWorld));

				std::cout << "Vertex " << j << ":";
				std::cout << std::endl;

				std::cout << "Position: " << "(" << i.position.GetX() << ", " << i.position.GetY() << ", " << i.position.GetZ() 
					<< ", " << i.position.GetW() << ")";
				std::cout << std::endl;

				std::cout << "Normal: " << "(" << i.normal.GetX() << ", " << i.normal.GetY() << ", " << i.normal.GetZ() 
					<< ", " << i.normal.GetW() << ")";
				std::cout << std::endl;

				FAMath::Vector4D pos2 = i.position + i.normal;
				std::cout << "2nd Position: " << "(" << pos2.GetX() << ", " << pos2.GetY() << ", " << pos2.GetZ() 
					<< ", " << pos2.GetW() << ")";
				std::cout << std::endl;

				std::cout << "Texture Coordinates: " << "(" << i.texCoords.GetX() << ", " << i.texCoords.GetY() << ")";
				std::cout << std::endl;

				std::cout << "World Position: " << "(" << worldPos.GetX() << ", " << worldPos.GetY() << ", " <<
					worldPos.GetZ() << ", " << worldPos.GetW() << ")";
				std::cout << std::endl;

				std::cout << "World Normal: " << "(" << worldNormal.GetX() << ", " << worldNormal.GetY() << ", " <<
					worldNormal.GetZ() << ", " << worldNormal.GetW() << ")";
				std::cout << std::endl;

				std::cout << std::endl;
				++j;
			}
		}
#endif


	protected:
		//Center of the 3D shape.
		FAMath::Vector4D mCenter;

		//Local axes of the 3D shape.
		FAMath::Vector4D mX;
		FAMath::Vector4D mY;
		FAMath::Vector4D mZ;

		//Color of the 3D shape.
		FAColor::Color mColor;

		//If true, updates the local to world matrix.
		bool mUpdateLocalToWorldlMatrix;

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
		virtual void CreateNormals();
	};
}