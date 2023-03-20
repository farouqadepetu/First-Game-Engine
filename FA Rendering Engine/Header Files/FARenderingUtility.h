#pragma once

/** @file FARenderingUtility.h
*	@brief File has static variables numFrames and current frame, function nextFrame() and struct DrawArguments under
*	the namespace FARender.
*/

/** @namespace FARender
*	@brief The namespace has utility functions and structs,
*	VertexBuffer, IndexBuffer, ConstantBuffer, DeviceResources, RenderScene and Text classes.
*/
namespace FARender
{
	static const unsigned int numFrames{ 3 };
	static unsigned int currentFrame{ 0 };

	/**@brief Update our current frame value to go to the next frame.
	*/
	void nextFrame();
}