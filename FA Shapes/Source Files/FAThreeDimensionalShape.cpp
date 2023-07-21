#include "FAThreeDimensionalShape.h"

namespace FAShapes
{
	ThreeDimensionalShape::ThreeDimensionalShape()
	{}

	void ThreeDimensionalShape::InitializeThreeDimensionalShape(const FAMath::Vector4D position, const FAMath::Quaternion orientation,
		const FAColor::Color& color)
	{
		mPosition = position;
		mOrientation = orientation;
		mColor = color;
	}

	const FAMath::Vector4D& ThreeDimensionalShape::GetPosition() const
	{
		return mPosition;
	}

	const FAMath::Quaternion& ThreeDimensionalShape::GetOrientation() const
	{
		return mOrientation;
	}

	const FAColor::Color& ThreeDimensionalShape::GetColor() const
	{
		return mColor;
	}

	const FAMath::Matrix4x4& ThreeDimensionalShape::GetModelMatrix()
	{
		return mModelMatrix;
	}

	const FADrawArguments::DrawArguments& ThreeDimensionalShape::GetDrawArguments() const
	{
		return mDrawArguments;
	}

	void ThreeDimensionalShape::SetPosition(const FAMath::Vector4D& position)
	{
		mPosition = position;
	}

	void ThreeDimensionalShape::SetOrientation(const FAMath::Quaternion& orientation)
	{
		mOrientation = orientation;
	}

	void ThreeDimensionalShape::SetModelMatrix(const FAMath::Matrix4x4& m)
	{
		mModelMatrix = m;
	}

	void ThreeDimensionalShape::SetDrawArguments(unsigned int indexCount, unsigned int locationFirstIndex, int indexFirstVertex, unsigned int indexConstantData,
		const std::wstring& constantBufferKey, unsigned int rootParameterIndex, D3D_PRIMITIVE_TOPOLOGY primitive)
	{
		mDrawArguments.indexCount = indexCount;
		mDrawArguments.locationOfFirstIndex = locationFirstIndex;
		mDrawArguments.indexOfFirstVertex = indexFirstVertex;
		mDrawArguments.indexOfConstantData = indexConstantData;
		mDrawArguments.constantBufferKey = constantBufferKey;
		mDrawArguments.rootParameterIndex = rootParameterIndex;
		mDrawArguments.primtive = primitive;
	}

	void ThreeDimensionalShape::UpdateShape(FARender::RenderScene* scene, const void* data, unsigned int size)
	{
		scene->CopyDataIntoDynamicBuffer(mDrawArguments.constantBufferKey, mDrawArguments.indexOfConstantData, data, (unsigned long long)size);
	}

	void ThreeDimensionalShape::RenderShape(FARender::RenderScene* scene)
	{
		scene->LinkDynamicBuffer(FARender::CONSTANT_BUFFER, mDrawArguments.constantBufferKey, mDrawArguments.indexOfConstantData,
			mDrawArguments.rootParameterIndex);

		scene->RenderObject(mDrawArguments.indexCount, mDrawArguments.locationOfFirstIndex, mDrawArguments.indexOfFirstVertex,
			mDrawArguments.primtive);
	}
}