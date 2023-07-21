#pragma once

#include "FARenderScene.h"
#include "FADrawArgumentsStructure.h"

namespace MessageLoop
{
	void FrameStats();
	void UserInput();
	void Update(FARender::RenderScene& scene);
	void Draw(FARender::RenderScene& scene);
}