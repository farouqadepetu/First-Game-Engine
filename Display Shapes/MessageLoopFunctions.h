#pragma once

#include "FARenderScene.h"

namespace MessageLoop
{
	void FrameStats(FARender::RenderScene& scene);
	void UserInput(FARender::RenderScene& scene);
	void Update(FARender::RenderScene& scene);
	void Draw(FARender::RenderScene& scene);
}