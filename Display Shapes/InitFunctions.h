#pragma once

#include "FASphere.h"
#include "FABox.h"
#include "FACylinder.h"
#include "FACone.h"
#include "FAPyramid.h"
#include "FARenderScene.h"
#include "FATime.h"

namespace Init
{
	//Init functions
	void BuildShapes();
	void BuildCamera(unsigned int width, unsigned int height);
	void BuildShaders(FARender::RenderScene& scene);
	void BuildVertexAndIndexList(FARender::RenderScene& scene);
	void BuildVertexAndIndexBuffers(FARender::RenderScene& scene);
	void BuildConstantBuffers(FARender::RenderScene& scene);
	void BuildPSOs(FARender::RenderScene& scene);
	void BuildText(unsigned int width, unsigned int height);
}