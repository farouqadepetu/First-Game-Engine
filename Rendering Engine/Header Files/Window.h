#pragma once

#include <Windows.h>
#include <string>
#include "Color.h"

namespace RenderingEngine
{
	/** @struct Window ""
	*	@brief The window struct is used to make a Window using Win32 API.
	*/
	struct Window
	{
		HWND windowHandle;
		WNDCLASSEX windowClass;
	};

	/**@brief Creates a parent window.
	*
	* The window gets displayed after it is created.
	*
	* @param[in] window A Window object.
	* 
	* @param[in] hInstance The handle to a module used to identify the executable.
	*
	* @param[in] windowProcedure The window procedure that is called when an event occurs.
	*
	* @param[in] backgroundColor The background color the window.
	*
	* @param[in] windowClassName The name of the window class.
	*
	* @param[in] windowName The name of the window.
	*
	* @param[in] styles The style of the window. OR together the styles at
	* https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
	*
	* @param[in] The x position of the top left corner of the window from the desktops top left corner.
	* Use CW_USEDEFAULT to let system select a default position for you.
	*
	* @param[in] The y position of the top left corner of the windo  from the desktops top left corner.
	* Use CW_USEDEFAULT to let system select a default position for you.
	*
	* @param[in] width The width of the client area of the window.
	*
	* @param[in] height The height of the client area of the window.
	*
	* @param[in, optional] additionalData A pointer to data to access in the window procedure.
	*
	*/
	void CreateParentWindow(Window& window, const HINSTANCE& hInstance, WNDPROC windowProcedure, const RenderingEngine::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

	/**@brief Creates a non-control child window.
	*
	* @param[in] window A Window object.
	* 
	* @param[in] hInstance The handle to a module used to identify the executable.
	*
	* @param[in] parent A handle to a parent window.
	*
	* @param[in] identifier An unsigned integer to identify the child window.
	*
	* @param[in] windowProcedure The window procedure that is called when an event occurs.
	*
	* @param[in] backgroundColor The background color the window.
	*
	* @param[in] windowClassName The name of the window class.
	*
	* @param[in] windowName The name of the window.
	*
	* @param[in] styles The style of the window. OR together the styles at
	* https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
	*
	* @param[in] The x position of the top left corner of the window from the parent window top left corner.
	*
	* @param[in] The y position of the top left corner of the window from the parent window top left corner..
	*
	* @param[in] width The width of the client area of the window.
	*
	* @param[in] height The height of the client area of the window.
	*
	* @param[in, optional] additionalData A pointer to data to access in the window procedure.
	*
	*/
	void CreateChildWindow(Window& window, const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
		WNDPROC windowProcedure, const RenderingEngine::Color& backgroundColor,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

	/**@brief Creates a control window.
	*
	* @param[in] window A Window object.
	* 
	* @param[in] hInstance The handle to a module used to identify the executable.
	*
	* @param[in] parent A handle to a parent window.
	*
	* @param[in] identifier An unsigned integer to identify the child window.
	*
	* @param[in] windowClass The name of the window class.
	*
	* @param[in] windowName The name of the window.
	*
	* @param[in] styles The style of the window. OR together the styles at
	* https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
	*
	* @param[in] The x position of the top left corner of the window from the parent window top left corner.
	*
	* @param[in] The y position of the top left corner of the window from the parent window top left corner.
	*
	* @param[in] width The width of the client area of the window.
	*
	* @param[in] height The height of the client area of the window.
	*
	* @param[in, optional] additionalData A pointer to data to access in the window procedure.
	*
	*/
	void CreateControlWindow(Window& window, const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
		const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
		unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

	/**brief Returns the width of the client area of the specified window.
	*/
	unsigned int GetWidth(const Window& window);

	/**brief Returns the height of the client area of the specified window.
	*/
	unsigned int GetHeight(const Window& window);

	/**brief Returns the x location of the specified window.
	*/
	unsigned int GetX(const Window& window);

	/**brief Returns the y location of the specified window.
	*/
	unsigned int GetY(const Window& window);
}