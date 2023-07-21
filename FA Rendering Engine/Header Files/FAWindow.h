#pragma once

/** @file FAWindow.h
*	@brief File that has namespace FAWindow. Withn the namespace is the class Window.
*/

#include <Windows.h>
#include <string>
#include <stdexcept>
#include "FAColor.h"

/** @namespace FAWindow
*	@brief Has Window class.
*/
namespace FAWindow
{
	/** @class Window ""
	*	@brief The window class is used to make a Window using Windows API.
	*	The copy constructor and assignment operators are explicitly deleted. This makes this class non-copyable.
	* 
	*/
	class Window
	{
	public:

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		/**@brief Creates a Window object. No window is created.
		* Call CreateParentWindow() or CreateChildWindow() or CreateControlWindow() to create a window.
		*/
		Window();

		/**@brief Creates a parent window.
		*
		* The window gets displayed after it is created.
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
		* @param[in, optional] parent a handle to a parent. Set to nullptr if it is not a child window.
		* 
		* @param[in, optional] additionalData A pointer to data to access in the window procedure.
		* 
		*/
		Window(const HINSTANCE& hInstance, WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
			const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
			unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates a non-control child window.
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
		Window(const HINSTANCE& hInstance, HWND parent, unsigned int identifier,
			WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
			const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
			unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates a control window.
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
		Window(const HINSTANCE& hInstance, HWND parent, unsigned int identifier,
			const std::wstring& windowClassName,
			const std::wstring& windowName, unsigned int styles,
			unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates a parent window.
		*
		* The window gets displayed after it is created.
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
		* @param[in, optional] parent a handle to a parent. Set to nullptr if it is not a child window.
		*
		* @param[in, optional] additionalData A pointer to data to access in the window procedure.
		*
		*/
		void CreateParentWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
			const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
			unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates a non-control child window.
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
		void CreateChildWindow(const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
			WNDPROC windowProcedure, const FAColor::Color& backgroundColor,
			const std::wstring& windowClassName, const std::wstring& windowName, unsigned int styles,
			unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates a control window.
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
		void CreateControlWindow(const HINSTANCE& hInstance, HWND parent, unsigned long long int identifier,
			const std::wstring& windowClassName,
			const std::wstring& windowName, unsigned int styles,
			unsigned int x, unsigned int y, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Returns the window handle.
		*/
		HWND GetWindowHandle() const;

		/**@brief Returns the width of the window.
		*/
		unsigned int GetWidth() const ;

		/**@brief Returns the height of the window.
		*/
		unsigned int GetHeight() const;

		/**@brief Returns the x position of the top left corner of the window.
		*/
		unsigned int GetX() const;

		/**@brief Returns the y position of the top left corner of the window.
		*/
		unsigned int GetY() const;

		/**@brief Sets the width of the window to the specified \a width.
		*/
		void SetWidth(unsigned int width);

		/**@brief Sets the height of the window o the specified \a height.
		*/
		void SetHeight(unsigned int height);

		/**@brief Sets the x position of the top left corner of the window.
		*/
		void SetX(unsigned int x);

		/**@brief Sets the y position of the top left corner of the window.
		*/
		void SetY(unsigned int y);

	private:
		HWND mWindowHandle;

		WNDCLASSEX mWindowClass;

		unsigned int mX;
		unsigned int mY;
		unsigned int mWidth;
		unsigned int mHeight;
	};
}