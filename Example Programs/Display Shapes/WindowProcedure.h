#pragma once

#include "Window.h"
#include "GameTime.h"
#include "RenderScene.h"

namespace WindowProc
{
	//Window procedure.
	LRESULT DisplayShapesWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ResizeText(unsigned int width, unsigned int height);
}