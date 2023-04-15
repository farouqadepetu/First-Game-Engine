#pragma once

#include "FAWindow.h"
#include "FATime.h"
#include "FARenderScene.h"

namespace WindowProc
{
	//Window procedure.
	LRESULT DisplayShapesWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ResizeText(unsigned int width, unsigned int height);

	//Resets all selections to their original selections
	void Reset(unsigned int width, unsigned int height);
}