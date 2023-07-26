#include "View.h"
#include <CommCtrl.h>
#include <strsafe.h>

namespace MVC
{
	View::View() : mIsMainWindowActive{ true }, mIsMainWindowMinimized{ false }, mIsMainWindowMaximized{ false },
		mCurrentShape{ 0 }, mCurrentTexture{ 0 }, mCurrentRenderOption{ 0 }, mCurrentLightSource{ 0 }, mRotateShape{ false },
		mRotatePointLight{ false }, mCameraMovement{ true }, textFont { nullptr }
	{}

	View::View(const HINSTANCE& hInstance, WNDPROC mainWindowProcedure, WNDPROC renderingWindowProcedure,
		void* mainWindowadditionalData, void* renderingWindowadditionalData)
		: mIsMainWindowActive{ true }, mIsMainWindowMinimized{ false }, mIsMainWindowMaximized{ false },
		mCurrentShape{ 0 }, mCurrentTexture{ 0 }, mCurrentRenderOption{ 0 }, mCurrentLightSource{ 0 }, mRotateShape{ false },
		mRotatePointLight{ false }, mCameraMovement{ true }, textFont{ nullptr }
	{
		CreateWindows(hInstance, mainWindowProcedure, renderingWindowProcedure, mainWindowadditionalData, renderingWindowadditionalData);
	}

	RenderingEngine::Window& View::GetMainWindow()
	{
		return mMainWindow;
	}

	RenderingEngine::Window& View::GetRenderingWindow()
	{
		return mRenderingWindow;
	}

	bool View::GetIsMainWindowActive()
	{
		return mIsMainWindowActive;
	}

	bool View::GetIsMainWindowMinimized()
	{
		return mIsMainWindowMinimized;
	}

	bool View::GetIsMainWindowMaximized()
	{
		return mIsMainWindowMaximized;
	}

	const unsigned int View::GetCurrentShape() const
	{
		return mCurrentShape;
	}

	const unsigned int View::GetCurrentTexture() const
	{
		return mCurrentTexture;
	}

	const unsigned int View::GetCurrentRenderOption() const
	{
		return mCurrentRenderOption;
	}

	const unsigned int View::GetCurrentLightSource() const
	{
		return mCurrentLightSource;
	}

	const bool View::GetRotateShape() const
	{
		return mRotateShape;
	}

	const bool View::GetRotatePointLight() const
	{
		return mRotatePointLight;
	}

	const bool View::GetCameraMovement() const
	{
		return mCameraMovement;
	}

	void View::SetCurrentShape(const unsigned int s)
	{
		mCurrentShape = s;
	}

	void View::SetCurrentTexture(const unsigned int t)
	{
		mCurrentTexture = t;
	}

	void View::SetCurrentRenderOption(const unsigned int r)
	{
		mCurrentRenderOption = r;
	}

	void View::SetCurrentLightSource(const unsigned int l)
	{
		mCurrentLightSource = l;
	}

	void View::RotateShape(bool rs)
	{
		mRotateShape = rs;
		if(rs)
			SetWindowText(mButtons[PLAY_PAUSE_SHAPE].windowHandle, L"Stop Rotating Shape");
		else
			SetWindowText(mButtons[PLAY_PAUSE_SHAPE].windowHandle, L"Rotate Shape");
	}

	void View::RotatePointLight(bool rl)
	{
		mRotatePointLight = rl;
		if (rl)
			SetWindowText(mButtons[PLAY_PAUSE_POINT_LIGHT].windowHandle, L"Stop Rotating Point Light(s)");
		else
			SetWindowText(mButtons[PLAY_PAUSE_POINT_LIGHT].windowHandle, L"Rotate Point Light(s)");
	}

	void View::CreateWindows(const HINSTANCE& hInstance, WNDPROC mainWindowProcedure, WNDPROC renderingWindowProcedure, 
		void* mainWindowadditionalData, void* renderingWindowadditionalData)
	{
		CreateMainWindow(hInstance, mainWindowProcedure, mainWindowadditionalData);
		CreateRenderingWindow(hInstance, renderingWindowProcedure, renderingWindowadditionalData);
		CreateDropDownLists(hInstance);
		CreateStaticText(hInstance);
		CreateButtons(hInstance);
	}

	void View::CreateMainWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData)
	{
		CreateParentWindow(mMainWindow, hInstance, windowProcedure, RenderingEngine::Color(0.4f, 0.4f, 0.4f), L"Main Window Class",
			L"Shading and Textures", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, additionalData);
	}

	void View::CreateRenderingWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData)
	{
		CreateChildWindow(mRenderingWindow,hInstance, mMainWindow.windowHandle, 0, windowProcedure, RenderingEngine::Color(),
			L"Rendering Window Class", L"", WS_CHILD | WS_VISIBLE, 0, 0, RenderingEngine::GetWidth(mMainWindow) - 200,
			RenderingEngine::GetHeight(mMainWindow), additionalData);
	}

	void View::CreateDropDownLists(const HINSTANCE& hInstance)
	{
		unsigned int dropDownListWidth{ RenderingEngine::GetWidth(mMainWindow) - RenderingEngine::GetWidth(mRenderingWindow) };
		unsigned int dropDownListHeight{ 110 };
		unsigned int dropDownListX{ RenderingEngine::GetWidth(mRenderingWindow) };
		unsigned int dropDownListY{ 25 };

		CreateControlWindow(mDropDownLists[SHAPES], hInstance, mMainWindow.windowHandle, SHAPES, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Sphere");
		SendMessage(mDropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Cylinder");
		SendMessage(mDropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Cone");
		SendMessage(mDropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Box");
		SendMessage(mDropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Pyramid");
		SendMessage(mDropDownLists[SHAPES].windowHandle, CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);

		dropDownListY += dropDownListHeight;
		CreateControlWindow(mDropDownLists[TEXTURES], hInstance, mMainWindow.windowHandle, TEXTURES, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[TEXTURES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Earth");
		SendMessage(mDropDownLists[TEXTURES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Moon");
		SendMessage(mDropDownLists[TEXTURES].windowHandle, CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Sun");
		SendMessage(mDropDownLists[TEXTURES].windowHandle, CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);

		dropDownListY += dropDownListHeight;
		CreateControlWindow(mDropDownLists[RENDER_OPTIONS], hInstance, mMainWindow.windowHandle, RENDER_OPTIONS, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[RENDER_OPTIONS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Textures + Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Textures + No Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Color + Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Color + No Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Wireframe");
		SendMessage(mDropDownLists[RENDER_OPTIONS].windowHandle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		dropDownListY += dropDownListHeight;
		CreateControlWindow(mDropDownLists[LIGHT_SOURCE], hInstance, mMainWindow.windowHandle, LIGHT_SOURCE, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"1 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"2 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"3 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"4 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].windowHandle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}

	void View::CreateStaticText(const HINSTANCE& hInstance)
	{
		//Font for the static texts
		HDC hdc = GetDC(mMainWindow.windowHandle);
		LOGFONT logF{ 0 };
		logF.lfHeight = -MulDiv(15, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logF.lfWeight = FW_BOLD;
		textFont = CreateFontIndirect(&logF);

		unsigned int staticTextWidth{ RenderingEngine::GetWidth(mMainWindow) - RenderingEngine::GetWidth(mRenderingWindow) };
		unsigned int staticTextHeight{ 20 };
		unsigned int staticTextX{ RenderingEngine::GetWidth(mRenderingWindow) };
		unsigned int staticTextY{ 0 };

		CreateControlWindow(mStaticText[SHAPES], hInstance, mMainWindow.windowHandle, SHAPES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[SHAPES].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[SHAPES].windowHandle, L"SHAPES");

		staticTextY += 110;
		CreateControlWindow(mStaticText[TEXTURES], hInstance, mMainWindow.windowHandle, TEXTURES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[TEXTURES].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[TEXTURES].windowHandle, L"TEXTURES");

		staticTextY += 110;
		CreateControlWindow(mStaticText[RENDER_OPTIONS], hInstance, mMainWindow.windowHandle, TEXTURES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[RENDER_OPTIONS].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[RENDER_OPTIONS].windowHandle, L"RENDER OPTIONS");

		staticTextY += 110;
		CreateControlWindow(mStaticText[LIGHT_SOURCE], hInstance, mMainWindow.windowHandle, TEXTURES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[LIGHT_SOURCE].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[LIGHT_SOURCE].windowHandle, L"POINT LIGHTS");

		ReleaseDC(mMainWindow.windowHandle, hdc);
	}

	void View::CreateButtons(const HINSTANCE& hInstance)
	{
		//Font for the button text
		HDC hdc = GetDC(mMainWindow.windowHandle);
		LOGFONT logF{ 0 };
		logF.lfHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logF.lfWeight = FW_BOLD;
		textFont = CreateFontIndirect(&logF);

		unsigned int buttonX{ RenderingEngine::GetWidth(mRenderingWindow) + 10 };
		unsigned int buttonY{ RenderingEngine::GetY(mDropDownLists[3]) + 50 };
		unsigned int buttonWidth{ RenderingEngine::GetWidth(mMainWindow) - RenderingEngine::GetWidth(mRenderingWindow) - 20 };
		unsigned int buttonHeight{ 35 };

		CreateControlWindow(mButtons[PLAY_PAUSE_SHAPE], hInstance, mMainWindow.windowHandle, PLAY_PAUSE_SHAPE,
			L"BUTTON", L"Rotate Shape",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight);

		SendMessage(mButtons[PLAY_PAUSE_SHAPE].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 5;
		CreateControlWindow(mButtons[PLAY_PAUSE_POINT_LIGHT], hInstance, mMainWindow.windowHandle, PLAY_PAUSE_POINT_LIGHT,
			L"BUTTON", L"Rotate Point Light(s)",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[PLAY_PAUSE_POINT_LIGHT].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 10;
		CreateControlWindow(mButtons[RESET_CAMERA], hInstance, mMainWindow.windowHandle, RESET_CAMERA,
			L"BUTTON", L"Reset Camera to Original Position",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[RESET_CAMERA].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 10;
		CreateControlWindow(mButtons[RESET_SHAPE], hInstance, mMainWindow.windowHandle, RESET_SHAPE,
			L"BUTTON", L"Reset Shape to Original Position",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[RESET_SHAPE].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 10;
		CreateControlWindow(mButtons[RESET_POINT_LIGHT], hInstance, mMainWindow.windowHandle, RESET_POINT_LIGHT,
			L"BUTTON", L"Reset Point Light(s) to Original Position(s)",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[RESET_POINT_LIGHT].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
	}

	void View::ActivateMainWindow()
	{
		mIsMainWindowActive = true;
	}

	void View::DeactivateMainWindow()
	{
		mIsMainWindowActive = false;
	}

	void View::MinimizeMainWindow()
	{
		mIsMainWindowMinimized = true;
	}

	void View::UnMinimizeMainWindow()
	{
		mIsMainWindowMinimized = false;
	}

	void View::MaximizeMainWindow()
	{
		mIsMainWindowMaximized = true;
	}

	void View::UnMaximizeMainWindow()
	{
		mIsMainWindowMaximized = false;
	}

	void View::ResizeRenderingWindow(unsigned int width, unsigned int height)
	{
		SetWindowPos(mRenderingWindow.windowHandle, nullptr, 0, 0, RenderingEngine::GetWidth(mMainWindow) - RenderingEngine::GetWidth(mDropDownLists[0]),
			RenderingEngine::GetHeight(mMainWindow), SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	void View::ResizeControlWindows()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			MoveWindow(mDropDownLists[i].windowHandle, RenderingEngine::GetWidth(mRenderingWindow), RenderingEngine::GetY(mDropDownLists[i]),
				RenderingEngine::GetWidth(mDropDownLists[i]), RenderingEngine::GetHeight(mDropDownLists[i]), TRUE);

			MoveWindow(mStaticText[i].windowHandle, RenderingEngine::GetWidth(mRenderingWindow), RenderingEngine::GetY(mStaticText[i]),
				RenderingEngine::GetWidth(mStaticText[i]), RenderingEngine::GetHeight(mStaticText[i]), TRUE);

			MoveWindow(mButtons[i].windowHandle, RenderingEngine::GetWidth(mRenderingWindow) + 10, RenderingEngine::GetY(mButtons[i]),
				RenderingEngine::GetWidth(mButtons[i]), RenderingEngine::GetHeight(mButtons[i]), TRUE);
		}

		for (unsigned int i = 0; i < 5; ++i)
		{
			MoveWindow(mButtons[i].windowHandle, RenderingEngine::GetWidth(mRenderingWindow) + 10, RenderingEngine::GetY(mButtons[i]),
				RenderingEngine::GetWidth(mButtons[i]), RenderingEngine::GetHeight(mButtons[i]), TRUE);
		}
	}

	void View::DisableCameraMovement()
	{
		mCameraMovement = false;
	}

	void View::EnableCameraMovement()
	{
		mCameraMovement = true;
	}
}