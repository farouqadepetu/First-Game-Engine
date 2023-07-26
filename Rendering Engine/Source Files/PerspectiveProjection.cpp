#include "PerspectiveProjection.h"

namespace RenderingEngine
{
	void SetProperties(PerspectiveProjection& perspective, float znear, float zfar, float vFov, float aspectRatio)
	{
		perspective.znear = znear;
		perspective.zfar = zfar;
		perspective.verticalFov = vFov;
		perspective.aspectRatio = aspectRatio;

	}

	void UpdateProjectionMatrix(PerspectiveProjection& perspective)
	{
		float angle{ perspective.verticalFov / 2.0f };
		angle = angle * PI / 180.0f;

		float inverseTan{ 1.0f / tan(angle) };

		float inverseAspectRatio{ 1.0f / perspective.aspectRatio };

		float inverseFMinusN{ 1.0f / (perspective.zfar - perspective.znear) };

		perspective.projectionMatrix.SetRow(0, vec4{ inverseAspectRatio * inverseTan, 0.0f, 0.0f, 0.0f });
		perspective.projectionMatrix.SetRow(1, vec4{ 0.0f, inverseTan, 0.0f, 0.0f });
		perspective.projectionMatrix.SetRow(2, vec4{ 0.0f, 0.0f, perspective.zfar * inverseFMinusN, 1.0f });
		perspective.projectionMatrix.SetRow(3, vec4{ 0.0f, 0.0f, -perspective.znear * perspective.zfar * inverseFMinusN, 0.0f });
	}
}