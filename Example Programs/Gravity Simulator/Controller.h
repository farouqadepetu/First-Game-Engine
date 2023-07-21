#pragma once

#include "Model.h"
#include "View.h"
#include "FACamera.h"
#include "FAProjection.h"
#include "FATime.h"
#include "Structures.h"
#include "FADirectXException.h"

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

		void LerpCamera();
		void SlerpCamera();

	private:
		bool mPauseApplication;
		bool mPlayAnimaton;

		View mView;

		Model mModel;

		std::unique_ptr<FARender::RenderScene> mScene;

		FATime::Time mRenderTime;

		FACamera::Camera mCamera;
		FAProjection::PerspectiveProjection mPerspectiveProjection;
	};
}