#pragma once

#include "FAWindow.h"
#include "FAColor.h"
#include "FARenderScene.h"
#include "Model.h"

namespace MVC
{
	enum DropDownListNames { SHAPES, TEXTURES, RENDER_OPTIONS, LIGHT_SOURCE };

	enum RenderOptions { TEXTURES_PLUS_SHADING, TEXTURES_PLUS_NO_SHADING, COLOR_PLUS_SHADING, COLOR_PLUS_NO_SHADING, WIREFRAME };

	enum ButtonNames { PLAY_PAUSE_SHAPE, PLAY_PAUSE_POINT_LIGHT, RESET_CAMERA, RESET_SHAPE, RESET_POINT_LIGHT };

	class View
	{
	public:

		View();

		View(const HINSTANCE& hInstance, WNDPROC mainWindowProcedure, WNDPROC renderingWindowProcedure,
			void* mainWindowadditionalData, void* renderingWindowadditionalData);

		FAWindow::Window& GetMainWindow();
		FAWindow::Window& GetRenderingWindow();

		bool GetIsMainWindowActive();
		bool GetIsMainWindowMinimized();
		bool GetIsMainWindowMaximized();

		const unsigned int GetCurrentShape() const;
		const unsigned int GetCurrentTexture() const;
		const unsigned int GetCurrentRenderOption() const;
		const unsigned int GetCurrentLightSource() const;
		const bool GetRotateShape() const;
		const bool GetRotatePointLight() const;
		const bool GetCameraMovement() const;

		void SetCurrentShape(const unsigned int s);
		void SetCurrentTexture(const unsigned int t);
		void SetCurrentRenderOption(const unsigned int r);
		void SetCurrentLightSource(const unsigned int l);
		void RotateShape(bool rs);
		void RotatePointLight(bool rl);

		void CreateWindows(const HINSTANCE& hInstance, WNDPROC mainWindowProcedure, WNDPROC renderingWindowProcedure,
			void* mainWindowadditionalData, void* renderingWindowadditionalData);

		void ActivateMainWindow();
		void DeactivateMainWindow();
		void MinimizeMainWindow();
		void UnMinimizeMainWindow();
		void MaximizeMainWindow();
		void UnMaximizeMainWindow();

		void ResizeRenderingWindow(unsigned int width, unsigned int height);
		void ResizeControlWindows();

		void DisableCameraMovement();
		void EnableCameraMovement();

	private:
		void CreateMainWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData);

		void CreateRenderingWindow(const HINSTANCE& hInstance, WNDPROC windowProcedure, void* additionalData);

		void CreateDropDownLists(const HINSTANCE& hInstance);

		void CreateStaticText(const HINSTANCE& hInstance);

		void CreateButtons(const HINSTANCE& hInstance);

	private:
		bool mIsMainWindowActive;
		bool mIsMainWindowMinimized;
		bool mIsMainWindowMaximized;

		FAWindow::Window mMainWindow;
		FAWindow::Window mRenderingWindow;

		FAWindow::Window mDropDownLists[4];
		FAWindow::Window mStaticText[4];
		FAWindow::Window mButtons[5];

		HFONT textFont;

		unsigned int mCurrentShape;
		unsigned int mCurrentTexture;
		unsigned int mCurrentRenderOption;
		unsigned int mCurrentLightSource;

		bool mRotateShape;
		bool mRotatePointLight;
		bool mCameraMovement;
	};
}