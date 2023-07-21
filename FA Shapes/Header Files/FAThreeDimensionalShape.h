#pragma once


#include "FAColor.h"
#include "FADrawArgumentsStructure.h"
#include "FARenderScene.h"

namespace FAShapes
{
	class ThreeDimensionalShape
	{
	public:

		/**@brief Default Constructor.
		* Call InitializeThreeDimensionalShape() to initialize the properties of the 3D shape.
		*/
		ThreeDimensionalShape();

		/**@brief Initializes the properties of the 3D shape.
		*/
		void InitializeThreeDimensionalShape(const FAMath::Vector4D position, const FAMath::Quaternion orientation,
			const FAColor::Color& color);

		/**@brief Returns the position of the 3D shape.
		*/
		const FAMath::Vector4D& GetPosition() const;

		/**@brief Returns the orientation of the 3D shape.
		*/
		const FAMath::Quaternion& GetOrientation() const;

		/**@brief Returns the color of the 3D shape.
		*/
		const FAColor::Color& GetColor() const;

		/**@brief Returns the model matrix of the 3D shape.
		*/
		const FAMath::Matrix4x4& GetModelMatrix();

		/**@brief Returns the draw arguments of the 3D shape.
		*/
		const FADrawArguments::DrawArguments& GetDrawArguments() const;

		/**@brief Sets the position of the 3D shape.
		*/
		void SetPosition(const FAMath::Vector4D& position);

		/**@brief Sets the orientation of the 3D shape.
		*/
		void SetOrientation(const FAMath::Quaternion& orientation);

		/**@brief Sets the model matrix of the 3D shape.
		*/
		void SetModelMatrix(const FAMath::Matrix4x4& m);

		/**@brief Sets the draw arguments used to render the 3D shape.
		*/
		void SetDrawArguments(unsigned int indexCount, unsigned int locationFirstIndex, int indexFirstVertex, unsigned int indexConstantData,
			const std::wstring& constantBufferKey, unsigned int rootParameterIndex, D3D_PRIMITIVE_TOPOLOGY primitive);

		/**@brief Updates the 3D shape constant data.
		*/
		void UpdateShape(FARender::RenderScene* scene, const void* data, unsigned int size);

		/**@brief Renders the 3D shape.
		*/
		void RenderShape(FARender::RenderScene* scene);

	private:
		FAMath::Vector4D mPosition;

		FAMath::Quaternion mOrientation;

		FAColor::Color mColor;

		FAMath::Matrix4x4 mModelMatrix;

		FADrawArguments::DrawArguments mDrawArguments;
	};
}