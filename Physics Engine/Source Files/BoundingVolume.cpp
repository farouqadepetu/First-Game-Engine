#include "BoundingVolume.h"

namespace PhysicsEngine
{
	const RenderingEngine::Color& BoundingVolumeAbstract::GetColor() const
	{
		return mRenderObject.color;
	}

	const RenderingEngine::DrawArguments& BoundingVolumeAbstract::GetDrawArguments() const
	{
		return mRenderObject.drawArguments;
	}

	const mat4& BoundingVolumeAbstract::GetModelMatrix() const
	{
		return mRenderObject.modelMatrix;
	}

	const vec3& BoundingVolumeAbstract::GetPosition() const
	{
		return mRenderObject.position;
	}

	const MathEngine::Quaternion& BoundingVolumeAbstract::GetOrientation() const
	{
		return mRenderObject.orientation;
	}

	void BoundingVolumeAbstract::SetPosition(const vec3& position)
	{
		mRenderObject.position = position;
	}

	void BoundingVolumeAbstract::SetOrientation(const MathEngine::Quaternion& orientation)
	{
		mRenderObject.orientation = orientation;
	}

	void BoundingVolumeAbstract::SetColor(const RenderingEngine::Color& color)
	{
		mRenderObject.color = color;
	}

	void BoundingVolumeAbstract::SetDrawArguments(const RenderingEngine::DrawArguments& drawArgs)
	{
		mRenderObject.drawArguments = drawArgs;
	}
}