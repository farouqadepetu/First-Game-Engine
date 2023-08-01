#include "RenderingEngineUtility.h"

namespace RenderingEngine
{
	void Update(RenderScene* scene, const DrawArguments& drawArgs, const void* data, unsigned int size)
	{
		scene->CopyDataIntoDynamicBuffer(drawArgs.constantBufferKey, drawArgs.indexOfConstantData, data, (unsigned long long)size);
	}

	void Render(RenderingEngine::RenderScene* scene, const DrawArguments& drawArgs)
	{
		scene->LinkDynamicBuffer(RenderingEngine::CONSTANT_BUFFER, drawArgs.constantBufferKey, drawArgs.indexOfConstantData,
			drawArgs.rootParameterIndex);

		scene->RenderObject(drawArgs.indexCount, drawArgs.locationOfFirstIndex, drawArgs.indexOfFirstVertex,
			drawArgs.primtive);
	}
}