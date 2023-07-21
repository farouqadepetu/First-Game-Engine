#pragma once

#include "FAWindow.h"

namespace MVC
{
	enum BUTTON_NAMES { PLAY_PAUSE = 1, RESET };

	class View
	{
	public:
		View(const HINSTANCE& hInstance, WNDPROC renderingWindowProcedure, WNDPROC settingsWindowProcedure, void* renderingWindowAdditionalData,
			void* settingsWindowAdditionalData);

		void CreateRenderingWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData);
		void CreateSettingsWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData);
		void CreatePlayPauseButton(const HINSTANCE& hInstance);
		void CreateResetButton(const HINSTANCE& hInstance);

		HWND GetRenderingWindowHandle();

		HWND GetPlayPauseButtonWindowHandle();

		unsigned int GetRenderingWindowWidth();

		unsigned int GetRenderingWindowHeight();

		HWND GetSettingsWindowHandle();

		void DisplaySettingsWindow();
		void HideSettingsWindow();

		void MinimizeRenderingWindow();
		void UnMinimizeRenderingWindow();

		void MaximizeRenderingWindow();
		void UnMaximizeRenderingWindow();

		void ResizingRenderingWindow();
		void NotResizingRenderingWindow();

		bool IsRenderingWindowMaximized();
		bool IsRenderingWindowMinimized();
		bool IsRenderingWindowBeingResized();
		bool IsSettingsWindowDisplayed();

		void ResizeRenderingWindow(unsigned int width, unsigned int height);
		void RepositionSettingsWindow();

	private:
	

	private:
		FAWindow::Window mRenderingWindow;
		FAWindow::Window mSettingsWindow;
		FAWindow::Window mPlayPauseButton;
		FAWindow::Window mResetButton;

		bool mDisplaySettingsWindow;
		bool mMinimizeRenderingWindow;
		bool mMaximizeRenderingWindow;
		bool RenderingWindowIsBeingResized;
	};
}