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
		Window();

		/**@brief Returns the window handle.
		*/
		HWND windowHandle() const;

		/**@brief Returns the width of the window.
		*/
		unsigned int width();

		/**@brief Returns the height of the window.
		*/
		unsigned int height();

		/**@brief Returns a reference to the bool variable that tells you if the window is minimized or not.
		*/
		bool& minimized();

		/**@brief Returns a reference to the bool variable that tells you if the window is maximized or not.
		*/
		bool& maximized();

		/**@brief Returns a reference to the bool variable that tells you if the window is being resized or not.
		*/
		bool& resizing();

		void registerWindowClass(const WNDCLASSEX& windowClass);

		/**@brief Registers the window class and creates the window.
		*/
		void createWindow(const HINSTANCE& hInstance, const std::wstring& windowName, unsigned int width, unsigned int height);

		/**@brief Updates and shows the window.
		*/
		void showWindow();

	private:
		HWND mWindowHandle;
		WNDCLASSEX mWindowClass;
		unsigned int mWidth;
		unsigned int mHeight;
		bool mMinimized;
		bool mMaximized;
		bool mResizing;
	};
}