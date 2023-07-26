#pragma once

#include "RenderScene.h"
#include "Time.h"
#include "Vertex.h"
#include "CreateShapes.h"

namespace Init
{
	//Init functions
	void BuildShapes();
	void BuildCamera();
	void BuildPerspectiveProjection(unsigned int width, unsigned int height);
	void BuildShaders(RenderingEngine::RenderScene& scene);
	void BuildVertexAndIndexList(RenderingEngine::RenderScene& scene);
	void BuildVertexAndIndexBuffers(RenderingEngine::RenderScene& scene);
	void BuildConstantBuffers(RenderingEngine::RenderScene& scene);
	void BuildPSOs(RenderingEngine::RenderScene& scene);
	void BuildText(unsigned int width, unsigned int height);
}