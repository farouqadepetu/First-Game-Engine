#pragma once

#include "Window.h"

namespace WindowProc
{
	//Window procedures
	LRESULT MainWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT RenderingWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
}