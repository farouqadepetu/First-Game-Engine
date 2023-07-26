#include "Window.h"

namespace RenderingEngine
{
	void CreateParentWindow(Window& window, const HINSTANCE& hInstance, WNDPROC windowProcedure, const RenderingEngine::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
	{
		window.windowClass.cbSize = sizeof(WNDCLASSEX);
		window.windowClass.style = CS_HREDRAW | CS_VREDRAW;
		window.windowClass.lpfnWndProc = windowProcedure;
		window.windowClass.cbClsExtra = 0;
		window.windowClass.cbWndExtra = 0;
		window.windowClass.hInstance = hInstance;
		window.windowClass.hIcon = nullptr;
		window.windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.windowClass.hbrBackground = CreateSolidBrush(RGB(backgroundColor.GetRed() * 255,
			backgroundColor.GetGreen() * 255, backgroundColor.GetBlue() * 255));
		window.windowClass.lpszMenuName = nullptr;
		window.windowClass.lpszClassName = windowClassName.c_str();
		window.windowClass.hIconSm = nullptr;

		if (!RegisterClassEx(&window.windowClass))
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

		window.windowHandle = CreateWindowEx(0, window.windowClass.lpszClassName, windowName.c_str(), styles,
			x, y, w, h, nullptr, nullptr, hInstance, additionalData);

		if (!window.windowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Creating Window Failed", MB_OK);
			exit(-1);
		}

		ShowWindow(window.windowHandle, SW_SHOW);
		UpdateWindow(window.windowHandle);
	}

	void CreateChildWindow(Window& window, const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
		WNDPROC windowProcedure, const RenderingEngine::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
	{
		window.windowClass.cbSize = sizeof(WNDCLASSEX);
		window.windowClass.style = CS_HREDRAW | CS_VREDRAW;
		window.windowClass.lpfnWndProc = windowProcedure;
		window.windowClass.cbClsExtra = 0;
		window.windowClass.cbWndExtra = 0;
		window.windowClass.hInstance = hInstance;
		window.windowClass.hIcon = nullptr;
		window.windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.windowClass.hbrBackground = CreateSolidBrush(RGB(backgroundColor.GetRed() * 255,
			backgroundColor.GetGreen() * 255, backgroundColor.GetBlue() * 255));
		window.windowClass.lpszMenuName = nullptr;
		window.windowClass.lpszClassName = windowClassName.c_str();
		window.windowClass.hIconSm = nullptr;

		if (!RegisterClassEx(&window.windowClass))
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "RegisterClassEx fucntion failed. Failed to register window class.\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Registering Window Class Failed", MB_OK);
			exit(-1);
		}

		window.windowHandle = CreateWindowEx(0, window.windowClass.lpszClassName, windowName.c_str(), styles,
			x, y, width, height, parent, (HMENU)identifier, hInstance, additionalData);

		if (!window.windowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Creating Window Failed", MB_OK);
			exit(-1);
		}
	}

	void CreateControlWindow(Window& window, const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData)
	{
		window.windowClass = WNDCLASSEX{};

		window.windowHandle = CreateWindowEx(0, windowClassName.c_str(), windowName.c_str(), styles,
			x, y, width, height, parent, (HMENU)identifier, hInstance, additionalData);

		if (!window.windowHandle)
		{
			DWORD errorCode = GetLastError();
			std::string errorMsg = "CreateWindow fucntion failed. Failed to create the window\nError Code: "
				+ std::to_string(errorCode);
			MessageBoxA(nullptr, errorMsg.c_str(), "Creating Window Failed", MB_OK);
			exit(-1);
		}
	}

	unsigned int GetWidth(const Window& window)
	{
		RECT clientAreaRect{};
		
		GetClientRect(window.windowHandle, &clientAreaRect);

		return clientAreaRect.right;
	}

	unsigned int GetHeight(const Window& window)
	{
		RECT clientAreaRect{};

		GetClientRect(window.windowHandle, &clientAreaRect);

		return clientAreaRect.bottom;
	}

	unsigned int GetX(const Window& window)
	{
		RECT windowAreaRect{};

		GetWindowRect(window.windowHandle, &windowAreaRect);

		if (GetParent(window.windowHandle) != nullptr)
		{
			//Transforms the rect coordinates from the desktop coordinates to the client area coordinates of its parent window.
			MapWindowPoints(HWND_DESKTOP, GetParent(window.windowHandle), (LPPOINT)&windowAreaRect, 2);
		}

		return windowAreaRect.left;
	}

	unsigned int GetY(const Window& window)
	{
		RECT windowAreaRect{};

		GetWindowRect(window.windowHandle, &windowAreaRect);

		if (GetParent(window.windowHandle) != nullptr)
		{
			//Transforms the rect coordinates from the desktop coordinates to the client area coordinates of its parent window.
			MapWindowPoints(HWND_DESKTOP, GetParent(window.windowHandle), (LPPOINT)&windowAreaRect, 2);
		}

		return windowAreaRect.top;
	}
}