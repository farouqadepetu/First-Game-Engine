#pragma once


#include "Color.h"
#include "DrawArguments.h"
#include "RenderScene.h"

/** @namespace ShapesEngine
* @brief An engine for rendering 3D shapes.
*/
namespace ShapesEngine
{
	/** @struct ThreeDimensionalShape ""
	*	@brief The struct stores the properties needed to render a 3D shape.
	*/
	struct ThreeDimensionalShape
	{
		vec3 position;

		MathEngine::Quaternion orientation;

		RenderingEngine::Color color;

		mat4 modelMatrix;

		RenderingEngine::DrawArguments drawArguments;
	};

	/**@brief Sets the draw arguments used to render the 3D shape.
	*/
	void SetDrawArguments(ThreeDimensionalShape& shape, unsigned int indexCount, unsigned int locationFirstIndex, int indexFirstVertex, 
		unsigned int indexConstantData, const std::wstring& constantBufferKey, unsigned int rootParameterIndex, D3D_PRIMITIVE_TOPOLOGY primitive);

	/**@brief Updates the 3D shapes constant data.
	*/
	void UpdateShape(const ThreeDimensionalShape& shape, RenderingEngine::RenderScene* scene, const void* data, unsigned int size);

	/**@brief Renders the 3D shape.
	*/
	void RenderShape(const ThreeDimensionalShape& shape, RenderingEngine::RenderScene* scene);

}