#include "Box.h"

namespace ShapesEngine
{
	Box::Box(float width, float height, float depth, const vec3& position, const MathEngine::Quaternion& orientation,
		const RenderingEngine::Color& color)
	{
		InitializeBox(width, height, depth, position, orientation, color);
	}

	void Box::InitializeBox(float width, float height, float depth, const vec3& position, const MathEngine::Quaternion& orientation,
		const RenderingEngine::Color& color)
	{
		mRenderObject.position = position;
		mRenderObject.orientation = orientation;
		mRenderObject.color = color;

		mWidth = (width <= 0.0f) ? 1.0f : width;
		mHeight = (height <= 0.0f) ? 1.0f : height;
		mDepth = (depth <= 0.0f) ? 1.0f : depth;
	}

	vec3 Box::GetDimensions() const
	{
		return vec3{ mWidth, mHeight, mDepth };
	}

	void Box::SetDimensions(const vec3& dimensions)
	{
		mWidth = (dimensions.x <= 0.0f) ? 1.0f : dimensions.x;
		mHeight = (dimensions.y <= 0.0f) ? 1.0f : dimensions.y;
		mDepth = (dimensions.z <= 0.0f) ? 1.0f : dimensions.z;
	}

	void Box::UpdateModelMatrix()
	{
		mRenderObject.modelMatrix = MathEngine::Scale4x4(mWidth, mHeight, mDepth) * MathEngine::QuaternionToRotationMatrixRow4x4(mRenderObject.orientation) *
			MathEngine::Translate(mRenderObject.position);
	}

	float Box::Volume() const
	{
		return mWidth * mHeight * mDepth;
	}
}