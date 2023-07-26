#include "ThreeDimensionalShape.h"

namespace ShapesEngine
{
	void SetDrawArguments(ThreeDimensionalShape& shape, unsigned int indexCount, unsigned int locationFirstIndex, int indexFirstVertex, 
		unsigned int indexConstantData, const std::wstring& constantBufferKey, unsigned int rootParameterIndex, D3D_PRIMITIVE_TOPOLOGY primitive)
	{
		shape.drawArguments.indexCount = indexCount;
		shape.drawArguments.locationOfFirstIndex = locationFirstIndex;
		shape.drawArguments.indexOfFirstVertex = indexFirstVertex;
		shape.drawArguments.indexOfConstantData = indexConstantData;
		shape.drawArguments.constantBufferKey = constantBufferKey;
		shape.drawArguments.rootParameterIndex = rootParameterIndex;
		shape.drawArguments.primtive = primitive;
	}

	void UpdateShape(const ThreeDimensionalShape& shape, RenderingEngine::RenderScene* scene, const void* data, unsigned int size)
	{
		scene->CopyDataIntoDynamicBuffer(shape.drawArguments.constantBufferKey, shape.drawArguments.indexOfConstantData, data, (unsigned long long)size);
	}

	void RenderShape(const ThreeDimensionalShape& shape, RenderingEngine::RenderScene* scene)
	{
		scene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, shape.drawArguments.constantBufferKey, shape.drawArguments.indexOfConstantData,
			shape.drawArguments.rootParameterIndex);

		scene->RenderObject(shape.drawArguments.indexCount, shape.drawArguments.locationOfFirstIndex, shape.drawArguments.indexOfFirstVertex,
			shape.drawArguments.primtive);
	}
}