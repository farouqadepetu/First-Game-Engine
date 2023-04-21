#include "FAWindow.h"

namespace FAWindow
{
	Window::Window(const HINSTANCE& hInstance, const WNDCLASSEX& windowClass, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
		: mX{ x }, mY{ y }, mWidth {width}, mHeight{ height }, mWindowClass{ windowClass }
	{
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

		mWindowHandle = CreateWindowEx(0, mWindowClass.lpszClassName, windowName.c_str(), styles,
			x, y, w, h, nullptr, nullptr, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);
	}

	Window::Window(const HINSTANCE& hInstance, HWND parent, unsigned int identifier,
		const WNDCLASSEX& windowClass, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData) :
		mX{ x }, mY{ y }, mWidth{ width }, mHeight{ height }, mWindowClass{ windowClass }
	{
		if (!RegisterClassEx(&mWindowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: "
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}

		mWindowHandle = CreateWindowEx(0, mWindowClass.lpszClassName, windowName.c_str(), styles,
			x, y, width, height, parent, (HMENU) identifier, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}
	}

	Window::Window(const HINSTANCE& hInstance, HWND parent, unsigned int identifier, const std::wstring& windowClassName,
		const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData) :
		mX{ x }, mY{ y }, mWidth{ width }, mHeight{ height }
	{
		mWindowClass = WNDCLASSEX{};

		mWindowHandle = CreateWindowEx(0, windowClassName.c_str(), windowName.c_str(), styles,
			x, y, width, height, parent, (HMENU)identifier, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			throw std::runtime_error(errorMsg);
		}
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

	unsigned int Window::GetX() const
	{
		return mX;
	}

	unsigned int Window::GetY() const
	{
		return mY;
	}

	void Window::SetWidth(unsigned int width)
	{
		mWidth = width;
	}

	void Window::SetHeight(unsigned int height)
	{
		mHeight = height;
	}

	void Window::SetX(unsigned int x)
	{
		mX = x;
	}

	void Window::SetY(unsigned int y)
	{
		mY = y;
	}
}