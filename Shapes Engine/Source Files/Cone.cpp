#include "Cone.h"

namespace ShapesEngine
{
	Cone::Cone(float radius, float height, const vec3& position, const MathEngine::Quaternion& orientation,
		const RenderingEngine::Color& color)
	{
		InitializeCone(radius, height, position, orientation, color);
	}

	void Cone::InitializeCone(float radius, float height, const vec3& position, const MathEngine::Quaternion& orientation,
		const RenderingEngine::Color& color)
	{
		mRenderObject.position = position;
		mRenderObject.orientation = orientation;
		mRenderObject.color = color;

		mRadius = (radius <= 0.0f) ? 1.0f : radius;
		mHeight = (height <= 0.0f) ? 1.0f : height;
	}

	vec3 Cone::GetDimensions() const
	{
		return vec3{ mRadius, mHeight, mRadius };
	}

	void Cone::SetDimensions(const vec3& dimensions)
	{
		mRadius = (dimensions.x <= 0.0f) ? 1.0f : dimensions.x;
		mHeight = (dimensions.y <= 0.0f) ? 1.0f : dimensions.y;
		mRadius = (dimensions.z <= 0.0f) ? 1.0f : dimensions.z;
	}

	void Cone::UpdateModelMatrix()
	{
		mRenderObject.modelMatrix = MathEngine::Scale4x4(mRadius, mHeight, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mRenderObject.orientation) *
			MathEngine::Translate(mRenderObject.position);
	}

	float Cone::Volume() const
	{
		return PI * mRadius * mRadius * (mHeight / 3.0f);
	}
}