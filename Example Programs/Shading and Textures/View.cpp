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

	FAWindow::Window& View::GetMainWindow()
	{
		return mMainWindow;
	}

	FAWindow::Window& View::GetRenderingWindow()
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
			SetWindowText(mButtons[PLAY_PAUSE_SHAPE].GetWindowHandle(), L"Stop Rotating Shape");
		else
			SetWindowText(mButtons[PLAY_PAUSE_SHAPE].GetWindowHandle(), L"Rotate Shape");
	}

	void View::RotatePointLight(bool rl)
	{
		mRotatePointLight = rl;
		if (rl)
			SetWindowText(mButtons[PLAY_PAUSE_POINT_LIGHT].GetWindowHandle(), L"Stop Rotating Point Light(s)");
		else
			SetWindowText(mButtons[PLAY_PAUSE_POINT_LIGHT].GetWindowHandle(), L"Rotate Point Light(s)");
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
		mMainWindow.CreateParentWindow(hInstance, windowProcedure, FAColor::Color(0.4f, 0.4f, 0.4f), L"Main Window Class",
			L"Shading and Textures", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, additionalData);
	}

	void View::CreateRenderingWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData)
	{
		mRenderingWindow.CreateChildWindow(hInstance, mMainWindow.GetWindowHandle(), 0, windowProcedure, FAColor::Color(), 
			L"Rendering Window Class", L"", WS_CHILD | WS_VISIBLE, 0, 0, mMainWindow.GetWidth() - 200, 
			mMainWindow.GetHeight(), additionalData);
	}

	void View::CreateDropDownLists(const HINSTANCE& hInstance)
	{
		unsigned int dropDownListWidth{ mMainWindow.GetWidth() - mRenderingWindow.GetWidth() };
		unsigned int dropDownListHeight{ 110 };
		unsigned int dropDownListX{ mRenderingWindow.GetWidth() };
		unsigned int dropDownListY{ 25 };

		mDropDownLists[SHAPES].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), SHAPES, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[SHAPES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Sphere");
		SendMessage(mDropDownLists[SHAPES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Cylinder");
		SendMessage(mDropDownLists[SHAPES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Cone");
		SendMessage(mDropDownLists[SHAPES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Box");
		SendMessage(mDropDownLists[SHAPES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Pyramid");
		SendMessage(mDropDownLists[SHAPES].GetWindowHandle(), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);

		dropDownListY += dropDownListHeight;
		mDropDownLists[TEXTURES].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), TEXTURES, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[TEXTURES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Earth");
		SendMessage(mDropDownLists[TEXTURES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Moon");
		SendMessage(mDropDownLists[TEXTURES].GetWindowHandle(), CB_ADDSTRING, (WPARAM) 0, (LPARAM) L"Sun");
		SendMessage(mDropDownLists[TEXTURES].GetWindowHandle(), CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);

		dropDownListY += dropDownListHeight;
		mDropDownLists[RENDER_OPTIONS].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), RENDER_OPTIONS, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[RENDER_OPTIONS].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Textures + Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Textures + No Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Color + Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Color + No Shading");
		SendMessage(mDropDownLists[RENDER_OPTIONS].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Wireframe");
		SendMessage(mDropDownLists[RENDER_OPTIONS].GetWindowHandle(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		dropDownListY += dropDownListHeight;
		mDropDownLists[LIGHT_SOURCE].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), LIGHT_SOURCE, WC_COMBOBOX, L"",
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
			dropDownListX, dropDownListY, dropDownListWidth, dropDownListHeight);

		SendMessage(mDropDownLists[LIGHT_SOURCE].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"1 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"2 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"3 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"4 Point Light");
		SendMessage(mDropDownLists[LIGHT_SOURCE].GetWindowHandle(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}

	void View::CreateStaticText(const HINSTANCE& hInstance)
	{
		//Font for the static texts
		HDC hdc = GetDC(mMainWindow.GetWindowHandle());
		LOGFONT logF{ 0 };
		logF.lfHeight = -MulDiv(15, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logF.lfWeight = FW_BOLD;
		textFont = CreateFontIndirect(&logF);

		unsigned int staticTextWidth{ mMainWindow.GetWidth() - mRenderingWindow.GetWidth() };
		unsigned int staticTextHeight{ 20 };
		unsigned int staticTextX{ mRenderingWindow.GetWidth() };
		unsigned int staticTextY{ 0 };

		mStaticText[SHAPES].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), SHAPES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[SHAPES].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[SHAPES].GetWindowHandle(), L"SHAPES");

		staticTextY += 110;
		mStaticText[TEXTURES].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), TEXTURES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[TEXTURES].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[TEXTURES].GetWindowHandle(), L"TEXTURES");

		staticTextY += 110;
		mStaticText[RENDER_OPTIONS].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), TEXTURES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[RENDER_OPTIONS].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[RENDER_OPTIONS].GetWindowHandle(), L"RENDER OPTIONS");

		staticTextY += 110;
		mStaticText[LIGHT_SOURCE].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), TEXTURES, L"STATIC", L"",
			WS_VISIBLE | WS_CHILD | SS_CENTER, staticTextX, staticTextY, staticTextWidth, staticTextHeight);

		SendMessage(mStaticText[LIGHT_SOURCE].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
		SetWindowText(mStaticText[LIGHT_SOURCE].GetWindowHandle(), L"POINT LIGHTS");

		ReleaseDC(mMainWindow.GetWindowHandle(), hdc);
	}

	void View::CreateButtons(const HINSTANCE& hInstance)
	{
		//Font for the button text
		HDC hdc = GetDC(mMainWindow.GetWindowHandle());
		LOGFONT logF{ 0 };
		logF.lfHeight = -MulDiv(11, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logF.lfWeight = FW_BOLD;
		textFont = CreateFontIndirect(&logF);

		unsigned int buttonX{ mRenderingWindow.GetWidth() + 10 };
		unsigned int buttonY{ mDropDownLists[3].GetY() + 50 };
		unsigned int buttonWidth{ mMainWindow.GetWidth() - mRenderingWindow.GetWidth() - 20 };
		unsigned int buttonHeight{ 35 };

		mButtons[PLAY_PAUSE_SHAPE].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), PLAY_PAUSE_SHAPE, 
			L"BUTTON", L"Rotate Shape",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight);

		SendMessage(mButtons[PLAY_PAUSE_SHAPE].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 5;
		mButtons[PLAY_PAUSE_POINT_LIGHT].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), PLAY_PAUSE_POINT_LIGHT,
			L"BUTTON", L"Rotate Point Light(s)",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[PLAY_PAUSE_POINT_LIGHT].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 10;
		mButtons[RESET_CAMERA].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), RESET_CAMERA,
			L"BUTTON", L"Reset Camera to Original Position",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[RESET_CAMERA].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 10;
		mButtons[RESET_SHAPE].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), RESET_SHAPE,
			L"BUTTON", L"Reset Shape to Original Position",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[RESET_SHAPE].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

		buttonY += buttonHeight + 10;
		mButtons[RESET_POINT_LIGHT].CreateControlWindow(hInstance, mMainWindow.GetWindowHandle(), RESET_POINT_LIGHT,
			L"BUTTON", L"Reset Point Light(s) to Original Position(s)",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_MULTILINE | BS_TEXT, buttonX, buttonY, buttonWidth, buttonHeight + 5);

		SendMessage(mButtons[RESET_POINT_LIGHT].GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
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
		mRenderingWindow.SetWidth(width);
		mRenderingWindow.SetHeight(height);

		MoveWindow(mRenderingWindow.GetWindowHandle(), 0, 0, mRenderingWindow.GetWidth(), mRenderingWindow.GetHeight(), FALSE);
	}

	void View::ResizeControlWindows()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			mDropDownLists[i].SetX(mRenderingWindow.GetWidth());
			MoveWindow(mDropDownLists[i].GetWindowHandle(), mDropDownLists[i].GetX(), mDropDownLists[i].GetY(),
				mDropDownLists[i].GetWidth(), mDropDownLists[i].GetHeight(), TRUE);

			mStaticText[i].SetX(mRenderingWindow.GetWidth());
			MoveWindow(mStaticText[i].GetWindowHandle(), mStaticText[i].GetX(), mStaticText[i].GetY(),
				mStaticText[i].GetWidth(), mStaticText[i].GetHeight(), TRUE);

			mButtons[i].SetX(mRenderingWindow.GetWidth() + 10);
			MoveWindow(mButtons[i].GetWindowHandle(), mButtons[i].GetX(), mButtons[i].GetY(),
				mButtons[i].GetWidth(), mButtons[i].GetHeight(), TRUE);
		}

		for (unsigned int i = 0; i < 5; ++i)
		{
			mButtons[i].SetX(mRenderingWindow.GetWidth() + 10);
			MoveWindow(mButtons[i].GetWindowHandle(), mButtons[i].GetX(), mButtons[i].GetY(),
				mButtons[i].GetWidth(), mButtons[i].GetHeight(), TRUE);
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