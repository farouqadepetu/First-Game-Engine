#include "FAWindow.h"

namespace FAWindow
{
	Window::Window() : mMinimized{ false }, mMaximized{ false }, mResizing{ false }
	{}

	HWND Window::windowHandle() const
	{
		return mWindowHandle;
	}

	unsigned int Window::width()
	{
		return mWidth;
	}

	unsigned int Window::height()
	{
		return mHeight;
	}

	bool& Window::minimized()
	{
		return mMinimized;
	}

	bool& Window::maximized()
	{
		return mMaximized;
	}

	bool& Window::resizing()
	{
		return mResizing;
	}

	void Window::registerWindowClass(const WNDCLASSEX& windowClass)
	{
		mWindowClass = windowClass;

		if (!RegisterClassEx(&mWindowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: " + std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}
	}

	void Window::createWindow(const HINSTANCE& hInstance, const std::wstring& windowName, unsigned int width, unsigned int height)
	{
		mWidth = width;
		mHeight = height;

		RECT desiredClientSize{ 0, 0, mWidth, mHeight };

		//Calulates the required size of the window to make sure the client window size is (WIDTH, HEIGHT)
		//When the function returns, the structure contains the coordinates of the top-left and bottom-right corners of the window 
		//to accommodate the desired client area.
		AdjustWindowRect(&desiredClientSize, WS_OVERLAPPEDWINDOW, false);

		int w = desiredClientSize.right - desiredClientSize.left;
		int h = desiredClientSize.bottom - desiredClientSize.top;

		mWindowHandle = CreateWindow(mWindowClass.lpszClassName, windowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w, h,
			nullptr, nullptr, hInstance, nullptr);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: " + std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}
	}

	void Window::showWindow()
	{
		ShowWindow(mWindowHandle, SW_SHOW);
		//UpdateWindow(mWindowHandle);
	}

}