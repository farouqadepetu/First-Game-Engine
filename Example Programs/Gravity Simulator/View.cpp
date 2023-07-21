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
		mRenderingWindow.CreateParentWindow(hInstance, windowProcedure, FAColor::Color(0.3f, 0.0f, 0.0f, 1.0f),
			L"Rendering Window Class", L"Gravity Simulator",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, additionalData);
	}

	void View::CreateSettingsWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData)
	{
		mSettingsWindow.CreateChildWindow(hInstance, mRenderingWindow.GetWindowHandle(), 0, windowProcedure, FAColor::Color(0.0f, 0.0f, 0.4f, 1.0f),
			L"Settings Window Class", L"Settings", WS_POPUP | WS_CAPTION | WS_SYSMENU,
			(mRenderingWindow.GetX() + mRenderingWindow.GetWidth()) / 3, (mRenderingWindow.GetY() + mRenderingWindow.GetHeight()) / 3,
			250, 250, additionalData);

		SetWindowPos(mSettingsWindow.GetWindowHandle(), nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	void View::CreatePlayPauseButton(const HINSTANCE& hInstance)
	{
		mPlayPauseButton.CreateControlWindow(hInstance, mSettingsWindow.GetWindowHandle(), PLAY_PAUSE, L"BUTTON", L"PLAY",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT,
			mSettingsWindow.GetWidth() / 10, mSettingsWindow.GetHeight() / 20, 75, 50);
	}

	void View::CreateResetButton(const HINSTANCE& hInstance)
	{
		mResetButton.CreateControlWindow(hInstance, mSettingsWindow.GetWindowHandle(), RESET, L"BUTTON", L"RESET",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT,
			mSettingsWindow.GetWidth() / 2, mSettingsWindow.GetHeight() / 20, 75, 50);
	}

	HWND View::GetRenderingWindowHandle()
	{
		return mRenderingWindow.GetWindowHandle();
	}

	HWND View::GetPlayPauseButtonWindowHandle()
	{
		return mPlayPauseButton.GetWindowHandle();
	}

	unsigned int View::GetRenderingWindowWidth()
	{
		return mRenderingWindow.GetWidth();
	}

	unsigned int View::GetRenderingWindowHeight()
	{
		return mRenderingWindow.GetHeight();
	}

	HWND View::GetSettingsWindowHandle()
	{
		return mSettingsWindow.GetWindowHandle();
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

	void View::ResizeRenderingWindow(unsigned int width, unsigned int height)
	{
		mRenderingWindow.SetWidth(width);
		mRenderingWindow.SetHeight(height);
	}

	void View::RepositionSettingsWindow()
	{
		unsigned int x = (mRenderingWindow.GetX() + mRenderingWindow.GetWidth()) / 3;
		unsigned int y = (mRenderingWindow.GetY() + mRenderingWindow.GetHeight()) / 3;
		SetWindowPos(mSettingsWindow.GetWindowHandle(), nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOZORDER);
	}
}