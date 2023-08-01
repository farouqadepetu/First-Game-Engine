#include "ThreeDimensionalShape.h"

namespace ShapesEngine
{
	const RenderingEngine::Color& ThreeDimensionalShapeAbstract::GetColor() const
	{
		return mRenderObject.color;
	}

	const RenderingEngine::DrawArguments& ThreeDimensionalShapeAbstract::GetDrawArguments() const
	{
		return mRenderObject.drawArguments;
	}

	const mat4& ThreeDimensionalShapeAbstract::GetModelMatrix() const
	{
		return mRenderObject.modelMatrix;
	}

	const vec3& ThreeDimensionalShapeAbstract::GetPosition() const
	{
		return mRenderObject.position;
	}

	const MathEngine::Quaternion& ThreeDimensionalShapeAbstract::GetOrientation() const
	{
		return mRenderObject.orientation;
	}

	void ThreeDimensionalShapeAbstract::SetPosition(const vec3& position)
	{
		mRenderObject.position = position;
	}

	void ThreeDimensionalShapeAbstract::SetOrientation(const MathEngine::Quaternion& orientation)
	{
		mRenderObject.orientation = orientation;
	}

	void ThreeDimensionalShapeAbstract::SetColor(const RenderingEngine::Color& color)
	{
		mRenderObject.color = color;
	}

	void ThreeDimensionalShapeAbstract::SetDrawArguments(const RenderingEngine::DrawArguments& drawArgs)
	{
		mRenderObject.drawArguments = drawArgs;
	}
}