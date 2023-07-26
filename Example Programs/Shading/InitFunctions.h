#pragma once

#include "Sphere.h"
#include "Box.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Pyramid.h"
#include "RenderScene.h"
#include "GameTime.h"
#include "GlobalVariables.h"

namespace Init
{
	//Init functions
	void BuildMainWindow(HINSTANCE hInstance);
	void BuildRenderingWindow(HINSTANCE hInstance);
	void BuildDropDownLists(HINSTANCE hInstance);
	void BuildButtons(HINSTANCE hInstance);
	void BuildMaterials();
	void BuildLightSources();
	void BuildShapes();
	void BuildCamera();
	void BuildPerspectiveProjection();
	void BuildScene();
	void BuildShaders();
	void BuildVertexAndIndexList();
	void BuildVertexAndIndexBuffers();
	void BuildConstantBuffers();
	void BuildPSOs();
	void BuildText();
}