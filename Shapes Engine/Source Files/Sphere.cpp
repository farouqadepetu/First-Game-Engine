#include "Sphere.h"

namespace ShapesEngine
{
	Sphere::Sphere(float radius, const vec3& position, const MathEngine::Quaternion& orientation,
		const RenderingEngine::Color& color)
	{
		InitializeSphere(radius, position, orientation, color);
	}

	void Sphere::InitializeSphere(float radius, const vec3& position, const MathEngine::Quaternion& orientation,
		const RenderingEngine::Color& color)
	{
		mRenderObject.position = position;
		mRenderObject.orientation = orientation;
		mRenderObject.color = color;

		mRadius = (radius <= 0.0f) ? 1.0f : radius;
	}

	vec3 Sphere::GetDimensions() const
	{
		return vec3{ mRadius, mRadius, mRadius };
	}

	void Sphere::SetDimensions(const vec3& dimensions)
	{
		mRadius = (dimensions.x <= 0.0f) ? 1.0f : dimensions.x;
	}

	void Sphere::UpdateModelMatrix()
	{
		mRenderObject.modelMatrix = MathEngine::Scale4x4(mRadius, mRadius, mRadius) * MathEngine::QuaternionToRotationMatrixRow4x4(mRenderObject.orientation) *
			MathEngine::Translate(mRenderObject.position);
	}

	float Sphere::Volume() const
	{
		return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
	}
}