#pragma once

#include <string>
#include <d3dcommon.h>

namespace FADrawArguments
{
	/** @struct DrawArguments ""
	*	@brief Data that are used as parameters to draw an object.
	*/
	struct DrawArguments
	{
		unsigned int indexCount = 0;
		unsigned int locationOfFirstIndex = 0;
		int indexOfFirstVertex = 0;
		unsigned int indexOfConstantData = 0;
		unsigned int rootParameterIndex = 0;
		std::wstring constantBufferKey = L"";
		D3D_PRIMITIVE_TOPOLOGY primtive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	};
}