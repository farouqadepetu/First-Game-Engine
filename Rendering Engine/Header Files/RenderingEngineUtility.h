#pragma once

#include "Color.h"
#include "DrawArguments.h"
#include "RenderScene.h"

namespace RenderingEngine
{
	/**brief Stores necessary data to render an object.
	*/
	struct RenderObject
	{
		vec3 position;

		MathEngine::Quaternion orientation;

		RenderingEngine::Color color;

		mat4 modelMatrix;

		RenderingEngine::DrawArguments drawArguments;
	};


	/**@brief Copies the speicified data in a constant buffer.
	*/
	void Update(RenderScene* scene, const DrawArguments& drawArgs, const void* data, unsigned int size);

	/**@brief Renders an object.
	*/
	void Render(RenderingEngine::RenderScene* scene, const DrawArguments& drawArgs);
}