#include "FAWindow.h"

namespace FAWindow
{
	Window::Window() : mWindowHandle{ nullptr }, mWindowClass{}, mX{ 0 }, mY{ 0 }, mWidth{ 0 }, mHeight{ 0 }
	{}

	Window::Window(const HINSTANCE& hInstance, WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
		: mX{ x }, mY{ y }, mWidth {width}, mHeight{ height }
	{
		CreateParentWindow(hInstance, windowProcedure, backgroundColor, windowClassName, windowName, styles, x, y, width, height,
			additionalData);
	}

	Window::Window(const HINSTANCE& hInstance, HWND parent, unsigned int identifier,
		WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData) :
		mX{ x }, mY{ y }, mWidth{ width }, mHeight{ height }
	{
		CreateChildWindow(hInstance, parent, identifier, windowProcedure, backgroundColor, windowClassName, windowName, styles,
			x, y, width, height, additionalData);
	}

	Window::Window(const HINSTANCE& hInstance, HWND parent, unsigned int identifier, const std::wstring& windowClassName,
		const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData) :
		mX{ x }, mY{ y }, mWidth{ width }, mHeight{ height }
	{
		CreateControlWindow(hInstance, parent, identifier, windowClassName, windowName, styles, x, y, width, height, additionalData);
	}

	void Window::CreateParentWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
	{
		mWindowClass.cbSize = sizeof(WNDCLASSEX);
		mWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		mWindowClass.lpfnWndProc = windowProcedure;
		mWindowClass.cbClsExtra = 0;
		mWindowClass.cbWndExtra = 0;
		mWindowClass.hInstance = hInstance;
		mWindowClass.hIcon = nullptr;
		mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		mWindowClass.hbrBackground = CreateSolidBrush(RGB(backgroundColor.GetRed() * 255,
			backgroundColor.GetGreen() * 255, backgroundColor.GetBlue() * 255));
		mWindowClass.lpszMenuName = nullptr;
		mWindowClass.lpszClassName = windowClassName.c_str();
		mWindowClass.hIconSm = nullptr;

		if (!RegisterClassEx(&mWindowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Registering Window Class Failed", MB_OK);
			exit(-1);
		}

		RECT desiredClientSize{ 0, 0, (long)width,(long)height };

		//Calulates the required size of the window to make sure the client window size is (WIDTH, HEIGHT)
		//When the function returns, the structure contains the coordinates of the top-left and bottom-right corners of the window 
		//to accommodate the desired client area.
		AdjustWindowRect(&desiredClientSize, styles, false);

		int w = desiredClientSize.right - desiredClientSize.left;
		int h = desiredClientSize.bottom - desiredClientSize.top;

		mWindowHandle = CreateWindowEx(0, mWindowClass.lpszClassName, windowName.c_str(), styles,
			x, y, w, h, nullptr, nullptr, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Creating Window Failed", MB_OK);
			exit(-1);
		}

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

		RECT r{};
		GetWindowRect(mWindowHandle, &r);

		mX = r.left;
		mY = r.top;
		mWidth = width;
		mHeight = height;
	}


	void Window::CreateChildWindow(const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
		WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
	{
		mWindowClass.cbSize = sizeof(WNDCLASSEX);
		mWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		mWindowClass.lpfnWndProc = windowProcedure;
		mWindowClass.cbClsExtra = 0;
		mWindowClass.cbWndExtra = 0;
		mWindowClass.hInstance = hInstance;
		mWindowClass.hIcon = nullptr;
		mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		mWindowClass.hbrBackground = CreateSolidBrush(RGB(backgroundColor.GetRed() * 255,
			backgroundColor.GetGreen() * 255, backgroundColor.GetBlue() * 255));
		mWindowClass.lpszMenuName = nullptr;
		mWindowClass.lpszClassName = windowClassName.c_str();
		mWindowClass.hIconSm = nullptr;

		if (!RegisterClassEx(&mWindowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Registering Window Class Failed", MB_OK);
			exit(-1);
		}

		mWindowHandle = CreateWindowEx(0, mWindowClass.lpszClassName, windowName.c_str(), styles,
			x, y, width, height, parent, (HMENU)identifier, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Creating Window Failed", MB_OK);
			exit(-1);
		}


		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
	}

	void Window::CreateControlWindow(const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
		const std::wstring& windowClassName,
		const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
	{
		mWindowClass = WNDCLASSEX{};

		mWindowHandle = CreateWindowEx(0, windowClassName.c_str(), windowName.c_str(), styles,
			x, y, width, height, parent, (HMENU)identifier, hInstance, additionalData);

		if (!mWindowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Creating Window Failed", MB_OK);
			exit(-1);
		}

		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
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