#pragma once

#include "RenderScene.h"
#include "DrawArguments.h"

namespace MessageLoop
{
	void FrameStats();
	void UserInput();
	void Update(RenderingEngine::RenderScene& scene);
	void Draw(RenderingEngine::RenderScene& scene);
}