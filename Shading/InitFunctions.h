#pragma once

#include "FASphere.h"
#include "FABox.h"
#include "FACylinder.h"
#include "FACone.h"
#include "FAPyramid.h"
#include "FARenderScene.h"
#include "FATime.h"
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
	void BuildScene();
	void BuildShaders();
	void BuildVertexAndIndexList();
	void BuildVertexAndIndexBuffers();
	void BuildConstantBuffers();
	void BuildPSOs();
	void BuildText();
}