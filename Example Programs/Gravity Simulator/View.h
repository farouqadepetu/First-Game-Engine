#pragma once

#include "Window.h"

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

		void RepositionSettingsWindow();

	private:
	

	private:
		RenderingEngine::Window mRenderingWindow;
		RenderingEngine::Window mSettingsWindow;
		RenderingEngine::Window mPlayPauseButton;
		RenderingEngine::Window mResetButton;

		bool mDisplaySettingsWindow;
		bool mMinimizeRenderingWindow;
		bool mMaximizeRenderingWindow;
		bool RenderingWindowIsBeingResized;
	};
}