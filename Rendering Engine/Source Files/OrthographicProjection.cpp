#include "OrthographicProjection.h"

namespace RenderingEngine
{
	void SetProperties(OrthogrpahicProjection& ortho, float width, float height, float znear, float zfar)
	{
		ortho.width = width;
		ortho.height = height;
		ortho.znear = znear;
		ortho.zfar = zfar;
	}

	void UpdateProjectionMatrix(OrthogrpahicProjection& ortho)
	{
		ortho.projectionMatrix.SetRow(0, vec4{ 2.0f / ortho.width, 0.0f, 0.0f, 0.0f });
		ortho.projectionMatrix.SetRow(1, vec4{ 0.0f, 2.0f / ortho.height, 0.0f, 0.0f });
		ortho.projectionMatrix.SetRow(2, vec4{ 0.0f, 0.0f, 1.0f / (ortho.zfar - ortho.znear), 0.0f });
		ortho.projectionMatrix.SetRow(3, vec4{ 0.0f, 0.0f, ortho.znear / (ortho.znear - ortho.zfar), 1.0f });
	}
}