#pragma once

#include "MathEngine.h"


namespace RenderingEngine
{
	/** @struct PerspectiveProjection ""
	*	@brief A struct that holds the properties for doing perspective projection.
	*/
	struct PerspectiveProjection
	{
		float znear = 0.0f;
		float zfar = 0.0f;
		float verticalFov = 0.0f;
		float aspectRatio = 0.0f;
		mat4 projectionMatrix;
	};

	/**@brief Sets the properties of the PerspectiveProjection object to the specified values.
	*
	* @param[in] perspective The PerspectiveProjection object.
	* @param[in] znear The z value of where the near plane of the frustrum intersects the z-axis.
	* @param[in] zfar The z value of where the far plane of the frustrum intersects the z-axis.
	* @param[in] vFov The vertical field of view of the frustrum.
	* @param[in] aspectRatio The aspect ratio of the view plane.
	*/
	void SetProperties(PerspectiveProjection& perspective, float znear, float zfar, float vFov, float aspectRatio);

	/**@brief Updates the perspective projection matrix of the PerspecitveProjection object.
	* 
	* @param[in] p The PerspectiveProjection object.
	*/
	void UpdateProjectionMatrix(PerspectiveProjection& perspective);
}