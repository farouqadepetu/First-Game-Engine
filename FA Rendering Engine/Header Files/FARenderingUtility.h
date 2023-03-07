#pragma once

/** @file FARenderingUtility.h
*	@brief File has static variables numFrames and current frame, function nextFrame() and struct DrawArguments under
*	the namespace FARender.
*/

#include <d3d12.h>

/** @namespace FARender
*	@brief The namespace has utility functions and structs,
*	VertexBuffer, IndexBuffer, ConstantBuffer, DeviceResources, RenderScene and Text classes.
*/
namespace FARender
{
	static const UINT numFrames{ 3 };
	static UINT currentFrame{ 0 };

	/**@brief Update our current frame value to go to the next frame.
	*/
	void nextFrame();

	/** @struct DrawArguments ""
	*	@brief Has all the data that are used as parameters to draw an object.
	*/
	struct DrawArguments
	{
		UINT indexCount;
		UINT locationFirstIndex;
		INT indexOfFirstVertex;
		INT indexOfConstantData;
	};
}