#pragma once

#include "MathEngine.h"

namespace RenderingEngine
{
	/** @struct OrthogrpahicProjection ""
	*	@brief A struct that holds the properties for doing orthographics projection.
	*/
	struct OrthogrpahicProjection
	{
		float width = 0.0f;
		float height = 0.0f;
		float znear = 0.0f;
		float zfar = 0.0f;
		mat4 projectionMatrix;
	};

	/**@brief Sets the properties of the OrthogrpahicProjection object to the specified values.
	*
	* @param[in] ortho The OrthogrpahicProjection object.
	* @param[in] width The width of the box.
	* @param[in] heigth The height of the box.
	* @param[in] znear The z value of the near plane.
	* @param[in] zfar The z value of the far plane.
	*/
	void SetProperties(OrthogrpahicProjection& ortho, float width, float height, float znear, float zfar);

	/**@brief Updates the perspective projection matrix of the PerspecitveProjection object.
	*
	* @param[in] p The PerspectiveProjection object.
	*/
	void UpdateProjectionMatrix(OrthogrpahicProjection& ortho);
}