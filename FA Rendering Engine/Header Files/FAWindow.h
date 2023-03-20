#pragma once

/** @file FAWindow.h
*	@brief File that has namespace FAWindow. Withn the namespace is the class Window.
*/

#include <Windows.h>
#include <string>
#include <stdexcept>

/** @namespace FAWindow
*	@brief Has Window class.
*/
namespace FAWindow
{
	/** @class Window ""
	*	@brief The window class is used to make a Window using Windows API.
	*/
	class Window
	{
	public:
		//Window();

		/**@brief Creates and displays a window.
		*  Registers a default window class with the OS with the specified instance, class name and window procdure.
		*/
		Window(const HINSTANCE& hInstance, const std::wstring& windowClassName, const std::wstring& windowName,
			WNDPROC winProcFunction, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates and displays a window.
		*  Registers the specified window class with the OS.
		*/
		Window(const HINSTANCE& hInstance, const WNDCLASSEX& windowClass, const std::wstring& windowName,
			unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Returns the window handle.
		*/
		HWND windowHandle() const;

		/**@brief Returns the width of the window.
		*/
		unsigned int width() const ;

		/**@brief Returns the height of the window.
		*/
		unsigned int height() const;

		/**@brief Sets the width of the window to the specified width.
		*/
		void setWidth(unsigned int width);

		/**@brief Sets the height of the window o the specified height.
		*/
		void setHeight(unsigned int height);

	private:
		HWND mWindowHandle;

		WNDCLASSEX mWindowClass;
		std::wstring mWindowClassName;

		unsigned int mWidth;
		unsigned int mHeight;
	};
}