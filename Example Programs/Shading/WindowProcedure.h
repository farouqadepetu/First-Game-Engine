#pragma once

#include "Window.h"
#include "GameTime.h"
#include "RenderScene.h"

namespace WindowProc
{
	//Window procedures
	LRESULT MainWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT RenderingWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ResizeText(unsigned int width, unsigned int height);
}