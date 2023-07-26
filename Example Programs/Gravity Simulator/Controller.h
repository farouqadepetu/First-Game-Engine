#pragma once

#include "Model.h"
#include "View.h"
#include "Camera.h"
#include "PerspectiveProjection.h"
#include "GameTime.h"
#include "Structures.h"
#include "DirectXException.h"

namespace MVC
{
	class Controller
	{
	public:
		Controller(const HINSTANCE& hInstance);

		int Run();

		friend LRESULT RenderingWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
		friend LRESULT SettingsWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		void LoadShaders();
		void CreateRootSignature();
		void CreatePSO();
		void CreateVertexAndIndexBuffers();
		void CreateConstantBuffer();

		void FrameStats();
		void CameraMovement();
		void Update();
		void Draw();

	private:
		bool mPauseApplication;
		bool mPlayAnimaton;

		View mView;

		Model mModel;

		std::unique_ptr<RenderingEngine::RenderScene> mScene;

		RenderingEngine::Time mRenderTime;

		RenderingEngine::Camera mCamera;
		RenderingEngine::PerspectiveProjection mPerspectiveProjection;

		vec2 mLastMousePosition;
	};
}