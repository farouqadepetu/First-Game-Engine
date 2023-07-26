#include "View.h"

namespace MVC
{
	View::View(const HINSTANCE& hInstance, WNDPROC renderingWindowProcedure, WNDPROC settingsWindowProcedure, void* renderingWindowAdditionalData,
		void* settingsWindowAdditionalData) :
		mDisplaySettingsWindow{ true }, mMinimizeRenderingWindow{ false }, mMaximizeRenderingWindow{ false }, RenderingWindowIsBeingResized{ false }
	{
		CreateRenderingWindow(hInstance, renderingWindowProcedure, renderingWindowAdditionalData);
		CreateSettingsWindow(hInstance, settingsWindowProcedure, settingsWindowAdditionalData);
		CreatePlayPauseButton(hInstance);
		CreateResetButton(hInstance);
	}

	void View::CreateRenderingWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData)
	{
		CreateParentWindow(mRenderingWindow, hInstance, windowProcedure, RenderingEngine::Color(0.3f, 0.0f, 0.0f, 1.0f),
			L"Rendering Window Class", L"Gravity Simulator",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, additionalData);
	}

	void View::CreateSettingsWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData)
	{
		CreateChildWindow(mSettingsWindow, hInstance, mRenderingWindow.windowHandle, 0, windowProcedure, RenderingEngine::Color(0.0f, 0.0f, 0.4f, 1.0f),
			L"Settings Window Class", L"Settings", WS_POPUP | WS_CAPTION | WS_SYSMENU,
			(RenderingEngine::GetX(mRenderingWindow) + RenderingEngine::GetWidth(mRenderingWindow)) / 3, 
			(RenderingEngine::GetY(mRenderingWindow) + RenderingEngine::GetHeight(mRenderingWindow)) / 3,
			250, 250, additionalData);

		SetWindowPos(mSettingsWindow.windowHandle, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	void View::CreatePlayPauseButton(const HINSTANCE& hInstance)
	{
		CreateControlWindow(mPlayPauseButton, hInstance, mSettingsWindow.windowHandle, PLAY_PAUSE, L"BUTTON", L"PLAY",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT,
			RenderingEngine::GetWidth(mSettingsWindow) / 10, RenderingEngine::GetHeight(mSettingsWindow) / 20, 75, 50);
	}

	void View::CreateResetButton(const HINSTANCE& hInstance)
	{
		CreateControlWindow(mResetButton, hInstance, mSettingsWindow.windowHandle, RESET, L"BUTTON", L"RESET",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT,
			RenderingEngine::GetWidth(mSettingsWindow) / 2, RenderingEngine::GetHeight(mSettingsWindow) / 20, 75, 50);
	}

	HWND View::GetRenderingWindowHandle()
	{
		return mRenderingWindow.windowHandle;
	}

	HWND View::GetPlayPauseButtonWindowHandle()
	{
		return mPlayPauseButton.windowHandle;
	}

	unsigned int View::GetRenderingWindowWidth()
	{
		return RenderingEngine::GetWidth(mRenderingWindow);
	}

	unsigned int View::GetRenderingWindowHeight()
	{
		return RenderingEngine::GetHeight(mRenderingWindow);
	}

	HWND View::GetSettingsWindowHandle()
	{
		return mSettingsWindow.windowHandle;
	}

	void View::DisplaySettingsWindow()
	{
		mDisplaySettingsWindow = true;
	}

	void View::HideSettingsWindow()
	{
		mDisplaySettingsWindow = false;
	}

	void View::MinimizeRenderingWindow()
	{
		mMinimizeRenderingWindow = true;
	}

	void View::UnMinimizeRenderingWindow()
	{
		mMinimizeRenderingWindow = false;
	}

	void View::MaximizeRenderingWindow()
	{
		mMaximizeRenderingWindow = true;
	}

	void View::UnMaximizeRenderingWindow()
	{
		mMaximizeRenderingWindow = false;
	}

	void View::ResizingRenderingWindow()
	{
		RenderingWindowIsBeingResized = true;
	}

	void View::NotResizingRenderingWindow()
	{
		RenderingWindowIsBeingResized = false;
	}

	bool View::IsRenderingWindowMaximized()
	{
		return mMaximizeRenderingWindow;
	}

	bool View::IsRenderingWindowMinimized()
	{
		return mMinimizeRenderingWindow;
	}

	bool View::IsRenderingWindowBeingResized()
	{
		return RenderingWindowIsBeingResized;
	}

	bool View::IsSettingsWindowDisplayed()
	{
		return mDisplaySettingsWindow;
	}

	void View::RepositionSettingsWindow()
	{
		unsigned int x = (RenderingEngine::GetX(mRenderingWindow) + RenderingEngine::GetWidth(mRenderingWindow)) / 3;
		unsigned int y = (RenderingEngine::GetY(mRenderingWindow) + RenderingEngine::GetHeight(mRenderingWindow)) / 3;
		SetWindowPos(mSettingsWindow.windowHandle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOZORDER);
	}
}