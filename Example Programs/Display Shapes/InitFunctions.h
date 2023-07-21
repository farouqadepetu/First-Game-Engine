#pragma once

#include "FARenderScene.h"
#include "FATime.h"
#include "FAVertexStructure.h"
#include "FAShapesUtility.h"

namespace Init
{
	//Init functions
	void BuildShapes();
	void BuildCamera();
	void BuildPerspectiveProjection(unsigned int width, unsigned int height);
	void BuildShaders(FARender::RenderScene& scene);
	void BuildVertexAndIndexList(FARender::RenderScene& scene);
	void BuildVertexAndIndexBuffers(FARender::RenderScene& scene);
	void BuildConstantBuffers(FARender::RenderScene& scene);
	void BuildPSOs(FARender::RenderScene& scene);
	void BuildText(unsigned int width, unsigned int height);
}