#include "FARenderingUtility.h"

namespace FARender
{
	void nextFrame()
	{
		currentFrame = (currentFrame + 1) % numFrames;
	}
}