#include "FAWindow.h"

namespace FAWindow
{
	Window::Window(const HINSTANCE& hInstance, const std::wstring& windowClassName, const std::wstring& windowName,
		WNDPROC winProcFunction, unsigned int width, unsigned int height, void* additionalData)
		: mWidth{ width }, mHeight{ height }, mWindowClassName{ windowClassName }
	{
		mWindowClass.cbSize = sizeof(WNDCLASSEX);
		mWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		mWindowClass.lpfnWndProc = winProcFunction;
		mWindowClass.cbClsExtra = 0;
		mWindowClass.cbWndExtra = 0;
		mWindowClass.hInstance = hInstance;
		mWindowClass.hIcon = nullptr;
		mWindowClass.hCursor = nullptr;
		mWindowClass.hbrBackground = nullptr;
		mWindowClass.lpszMenuName = nullptr;
		mWindowClass.lpszClassName = mWindowClassName.c_str();
		mWindowClass.hIconSm = nullptr;

		if (!RegisterClassEx(&mWindowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: " 
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}

		RECT desiredClientSize{ 0, 0, (long)mWidth, (long)mHeight };

		//Calulates the required size of the window to make sure the client window size is (WIDTH, HEIGHT)
		//When the function returns, the structure contains the coordinates of the top-left and bottom-right corners of the window 
		//to accommodate the desired client area.
		AdjustWindowRect(&desiredClientSize, WS_OVERLAPPEDWINDOW, false);

		int w = desiredClientSize.right - desiredClientSize.left;
		int h = desiredClientSize.bottom - desiredClientSize.top;

		mWindowHandle = CreateWindowEx(0, mWindowClass.lpszClassName, windowName.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, w, h, nullptr, nullptr, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: " 
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}

		ShowWindow(mWindowHandle, SW_SHOW);
	}


	Window::Window(const HINSTANCE& hInstance, const WNDCLASSEX& windowClass, const std::wstring& windowName,
		unsigned int width, unsigned int height, void* additionalData)
		: mWidth{ width }, mHeight{ height }, mWindowClass{ windowClass }
	{
		mWindowClassName = windowClass.lpszClassName;

		if (!RegisterClassEx(&mWindowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: " 
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}

		RECT desiredClientSize{ 0, 0, (long)mWidth,(long)mHeight };

		//Calulates the required size of the window to make sure the client window size is (WIDTH, HEIGHT)
		//When the function returns, the structure contains the coordinates of the top-left and bottom-right corners of the window 
		//to accommodate the desired client area.
		AdjustWindowRect(&desiredClientSize, WS_OVERLAPPEDWINDOW, false);

		int w = desiredClientSize.right - desiredClientSize.left;
		int h = desiredClientSize.bottom - desiredClientSize.top;

		mWindowHandle = CreateWindowEx(0, mWindowClass.lpszClassName, windowName.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, w, h, nullptr, nullptr, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: " 
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}

		ShowWindow(mWindowHandle, SW_SHOW);
	}

	HWND Window::GetWindowHandle() const
	{
		return mWindowHandle;
	}

	unsigned int Window::GetWidth() const
	{
		return mWidth;
	}

	unsigned int Window::GetHeight() const
	{
		return mHeight;
	}

	void Window::SetWidth(unsigned int width)
	{
		mWidth = width;
	}

	void Window::SetHeight(unsigned int height)
	{
		mHeight = height;
	}
}