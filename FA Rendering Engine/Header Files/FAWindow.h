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

		Window() = default;

		/**@brief Creates and displays a window.
		* 
		* @param[in] hInstance The handle to a module used to identify the executable.
		* @param[in] windowClassName The name of the window class.
		* @param[in] windowName The name of the window.
		* @param[in] winProcFunction The window procedure.
		* @param[in] width The width of the window.
		* @param[in] height The height of the window.
		* @param[in, optional] additionalData A pointer to data to access in the window procedure.
		*/
		Window(const HINSTANCE& hInstance, const std::wstring& windowClassName, const std::wstring& windowName,
			WNDPROC winProcFunction, unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Creates and displays a window.
		* 
		* @param[in] hInstance The handle to a module used to identify the executable.
		* @param[in] windowClass The window class for this window.
		* @param[in] windowName The name of the window.
		* @param[in] width The width of the window.
		* @param[in] height The height of the window.
		* @param[in, optional] additionalData A pointer to data to access in the window procedure.
		*/
		Window(const HINSTANCE& hInstance, const WNDCLASSEX& windowClass, const std::wstring& windowName,
			unsigned int width, unsigned int height, void* additionalData = nullptr);

		/**@brief Returns the window handle.
		*/
		HWND GetWindowHandle() const;

		/**@brief Returns the width of the window.
		*/
		unsigned int GetWidth() const ;

		/**@brief Returns the height of the window.
		*/
		unsigned int GetHeight() const;

		/**@brief Sets the width of the window to the specified \a width.
		*/
		void SetWidth(unsigned int width);

		/**@brief Sets the height of the window o the specified \a height.
		*/
		void SetHeight(unsigned int height);

	private:
		HWND mWindowHandle;

		WNDCLASSEX mWindowClass;
		std::wstring mWindowClassName;

		unsigned int mWidth;
		unsigned int mHeight;
	};
}