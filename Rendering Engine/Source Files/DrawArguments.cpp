#include "DrawArguments.h"

namespace RenderingEngine
{
	DrawArguments MakeDrawArguments(unsigned int indexCount, unsigned int locationFirstIndex, int indexFirstVertex,
		unsigned int indexConstantData, const std::wstring& constantBufferKey, unsigned int rootParameterIndex, D3D_PRIMITIVE_TOPOLOGY primitive)
	{
		DrawArguments drawArgs{};
		drawArgs.indexCount = indexCount;
		drawArgs.locationOfFirstIndex = locationFirstIndex;
		drawArgs.indexOfFirstVertex = indexFirstVertex;
		drawArgs.indexOfConstantData = indexConstantData;
		drawArgs.constantBufferKey = constantBufferKey;
		drawArgs.rootParameterIndex = rootParameterIndex;
		drawArgs.primtive = primitive;
		
		return drawArgs;
	}
}