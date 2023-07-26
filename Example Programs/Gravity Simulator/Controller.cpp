#include "Controller.h"
#include <iomanip>
#include <sstream>

namespace MVC
{
	LRESULT RenderingWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MVC::Controller* con{ nullptr };
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			con = (MVC::Controller*)pCreate->lpCreateParams;
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)con);
		}
		else
		{
			LONG_PTR ptr = GetWindowLongPtr(windowHandle, GWLP_USERDATA);
			con = (MVC::Controller*)ptr;
		}

		switch (uMsg)
		{
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				con->mPauseApplication = true;
				RenderingEngine::Stop(con->mRenderTime);
			}
			else
			{
				con->mPauseApplication = false;
				RenderingEngine::Start(con->mRenderTime);
			}

			return 0;
		}

		case WM_SIZE:
		{
			unsigned int newWidth = LOWORD(lParam);
			unsigned int newHeight = HIWORD(lParam);

			con->mPerspectiveProjection.aspectRatio = (float)newWidth / newHeight;

			if (wParam == SIZE_MAXIMIZED)
			{
				con->mView.MaximizeRenderingWindow();
				con->mView.MinimizeRenderingWindow();

				con->mScene->Resize(newWidth, newHeight, windowHandle);

			}
			else if (wParam == SIZE_MINIMIZED)
			{
				con->mPauseApplication = true;
				con->mView.UnMaximizeRenderingWindow();
				con->mView.UnMinimizeRenderingWindow();

				RenderingEngine::Stop(con->mRenderTime);

			}
			else if (wParam == SIZE_RESTORED)
			{
				if (con->mView.IsRenderingWindowMinimized())
				{
					con->mView.UnMinimizeRenderingWindow();

					con->mScene->Resize(newWidth, newHeight, windowHandle);

					RenderingEngine::Start(con->mRenderTime);
				}
				else if (con->mView.IsRenderingWindowMaximized())
				{
					con->mView.UnMaximizeRenderingWindow();

					con->mScene->Resize(newWidth, newHeight, windowHandle);
				}
				else if (con->mView.IsRenderingWindowBeingResized())
				{
					//DO NOTHING IF THE WINDOW IS CURRENTLY BEING RESIZED
				}
				else
				{
					if (con->mScene != nullptr)
					{
						con->mScene->Resize(newWidth, newHeight, windowHandle);
					}
				}
			}


			return 0;
		}

		case WM_ENTERSIZEMOVE:
		{
			con->mPauseApplication = true;
			RenderingEngine::Stop(con->mRenderTime);
			con->mView.ResizingRenderingWindow();
			return 0;
		}

		case WM_EXITSIZEMOVE:
		{
			con->mPauseApplication = false;
			RenderingEngine::Start(con->mRenderTime);
			con->mView.NotResizingRenderingWindow();

			RECT renderingWindowClientAreaRect{};

			GetClientRect(windowHandle, &renderingWindowClientAreaRect);

			unsigned int newWidth = renderingWindowClientAreaRect.right;
			unsigned int newHeight = renderingWindowClientAreaRect.bottom;

			con->mScene->Resize(newWidth, newHeight, windowHandle);

			con->mPerspectiveProjection.aspectRatio = (float)newWidth / newHeight;

			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
			{
				SendMessage(con->mView.GetSettingsWindowHandle(), uMsg, wParam, lParam);

				return 0;
			}
			}

			return 0;
		}
		}

		return DefWindowProc(windowHandle, uMsg, wParam, lParam);
	}

	LRESULT SettingsWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MVC::Controller* con{ nullptr };
		if (uMsg == WM_CREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			con = (MVC::Controller*)pCreate->lpCreateParams;
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)con);
		}
		else
		{
			LONG_PTR ptr = GetWindowLongPtr(windowHandle, GWLP_USERDATA);
			con = (MVC::Controller*)ptr;
		}

		switch (uMsg)
		{
		case WM_CLOSE:
		{
			ShowWindow(con->mView.GetSettingsWindowHandle(), SW_HIDE);
			con->mView.HideSettingsWindow();
			return 0;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
			{
				if (con->mView.IsSettingsWindowDisplayed())
				{
					ShowWindow(windowHandle, SW_HIDE);
					con->mView.HideSettingsWindow();
				}
				else
				{
					con->mView.RepositionSettingsWindow();
					con->mView.DisplaySettingsWindow();
				}

				break;
			}
			}

			return 0;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (LOWORD(wParam) == PLAY_PAUSE)
				{
					if (con->mPlayAnimaton)
					{
						con->mPlayAnimaton = false;
						SetWindowText((HWND)lParam, L"PLAY");
					}
					else
					{
						con->mPlayAnimaton = true;
						SetWindowText((HWND)lParam, L"PAUSE");
					}

					SetFocus(con->mView.GetRenderingWindowHandle());
				}
				else if (LOWORD(wParam) == RESET)
				{
					con->mModel.Reset();
					con->mPlayAnimaton = false;
					SetWindowText(con->mView.GetPlayPauseButtonWindowHandle(), L"PLAY");
					SetFocus(con->mView.GetRenderingWindowHandle());
				}
			}

			return 0;
		}
		}

		return DefWindowProc(windowHandle, uMsg, wParam, lParam);
	}

	Controller::Controller(const HINSTANCE& hInstance) : mPauseApplication{ false }, mPlayAnimaton{ false },
		mView(hInstance, RenderingWindowProc, SettingsWindowProc, this, this)
	{
		mScene = std::make_unique<RenderingEngine::RenderScene>(mView.GetRenderingWindowWidth(), mView.GetRenderingWindowHeight(),
			mView.GetRenderingWindowHandle());

		LoadShaders();
		CreateRootSignature();
		CreatePSO();
		CreateVertexAndIndexBuffers();
		CreateConstantBuffer();

		mScene->ExecuteAndFlush();

		SetProperties(mCamera, vec3{ 10.0f, 0.0f, -25.0f }, vec3{ 1.0f, 0.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.0f }, vec3{ 0.0f, 0.0f, 1.0f },
			5.0f, 0.125f);

		SetProperties(mPerspectiveProjection, 1.0f, 1000.0f, 45.0f, (float)mView.GetRenderingWindowWidth() / mView.GetRenderingWindowHeight());
	}

	void Controller::LoadShaders()
	{
		mScene->CompileShader(L"GS Vertex Shader", L"GravitySimulatorVS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(L"GS Pixel Shader", L"GravitySimulatorPS.hlsl", "psMain", "ps_5_1");

		mScene->CreateInputElementDescription(L"GS VS Input", "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
		mScene->CreateInputElementDescription(L"GS VS Input", "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12);
		mScene->CreateInputElementDescription(L"GS VS Input", "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24);

		mScene->CreateRootDescriptor(L"GS Root Parameter", 0);
	}

	void Controller::CreateRootSignature()
	{
		mScene->CreateRootSignature(L"GS Root Sig", L"GS Root Parameter");
	}

	void Controller::CreatePSO()
	{
		mScene->CreatePSO(L"GS PSO WIREFRAME", D3D12_FILL_MODE_WIREFRAME, FALSE, L"GS Vertex Shader", L"GS Pixel Shader", L"GS VS Input", L"GS Root Sig",
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		mScene->CreatePSO(L"GS PSO SOLID", D3D12_FILL_MODE_SOLID, FALSE, L"GS Vertex Shader", L"GS Pixel Shader", L"GS VS Input", L"GS Root Sig",
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}

	void Controller::CreateVertexAndIndexBuffers()
	{
		mModel.StoreVerticesAndIndices(mScene.get());
	}

	void Controller::CreateConstantBuffer()
	{
		mScene->CreateDynamicBuffer(L"Object Constant Buffer", 10 * sizeof(ObjectConstants), nullptr, sizeof(ObjectConstants));
	}

	void Controller::CameraMovement()
	{
		//Poll keyboard and mouse input.
		//check if w, a, s, d or up, left, right, down, spacebar or control was pressed

		if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
			RenderingEngine::Foward(mCamera, (float)mRenderTime.deltaTime);
		if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
			RenderingEngine::Left(mCamera, (float)mRenderTime.deltaTime);
		if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
			RenderingEngine::Backward(mCamera, (float)mRenderTime.deltaTime);
		if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
			RenderingEngine::Right(mCamera, (float)mRenderTime.deltaTime);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			RenderingEngine::Up(mCamera, (float)mRenderTime.deltaTime);
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			RenderingEngine::Down(mCamera, (float)mRenderTime.deltaTime);

		POINT currMousePos{};
		GetCursorPos(&currMousePos);

		vec2 currentMousePosition{ (float)currMousePos.x, (float)currMousePos.y };

		vec2 mousePositionDiff{ currentMousePosition - mLastMousePosition };

		//if the mouse goes outside the window and comes back into the window, the camera won't be rotated.
		if (Length(mousePositionDiff) < 10.0f && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			RenderingEngine::RotateCameraLeftRight(mCamera, mCamera.angularSpeed * mousePositionDiff.x);
			RenderingEngine::RotateCameraUpDown(mCamera, mCamera.angularSpeed * mousePositionDiff.y);
		}

		mLastMousePosition = currentMousePosition;
	}

	void Controller::Update()
	{
		RenderingEngine::UpdateViewMatrix(mCamera);
		RenderingEngine::UpdateProjectionMatrix(mPerspectiveProjection);

		if(mPlayAnimaton)
			mModel.Simulate((float)mRenderTime.deltaTime);

		mModel.UpdateModels(mScene.get(), mCamera.viewMatrix, mPerspectiveProjection.projectionMatrix);
	}

	void Controller::Draw()
	{
		mScene->BeforeRenderObjects();

		mScene->LinkPSOAndRootSignature(L"GS PSO WIREFRAME", L"GS Root Sig");

		mScene->LinkStaticBuffer(RenderingEngine::VERTEX_BUFFER, L"Vertex Buffer");
		mScene->LinkStaticBuffer(RenderingEngine::INDEX_BUFFER, L"Index Buffer");

		mModel.RenderModels(mScene.get());

		mScene->AfterRenderObjects();

		mScene->AfterRender();
	}

	void Controller::FrameStats()
	{
		//computes average frames per second and
		//the average time it takes to show each frame

		static unsigned int frameCount{ 0 };
		static float timeElapsed{ 0.0f };

		++frameCount;

		timeElapsed += (float)mRenderTime.deltaTime;
		if (timeElapsed >= 1.0f)
		{
			float fps = (float)frameCount; //fps = number of frames / 1 second
			float milliSecondsPerFrame = 1000.0f / fps;

			std::stringstream fpsStream;
			fpsStream << std::setprecision(6) << fps;
			std::string fpsString{ fpsStream.str() };

			std::stringstream milliSecondsPerFrameStream;
			milliSecondsPerFrameStream << std::setprecision(6) << milliSecondsPerFrame;
			std::string milliSecondsPerFrameString{ milliSecondsPerFrameStream.str() };

			std::wstring fpsWString{ AnsiToWString(fpsString) };
			std::wstring milliSecondsPerFrameWString{ AnsiToWString(milliSecondsPerFrameString) };
			std::wstring textStr = L"Gravity Simulator - FPS: " + fpsWString + L"     Frame Time: " + milliSecondsPerFrameWString;
			SetWindowText(mView.GetRenderingWindowHandle(), textStr.c_str());

			//reset for next average
			frameCount = 0;
			timeElapsed = 0.0f;
		}
	}

	int Controller::Run()
	{
		MSG msg{};

		RenderingEngine::InitializeTime(mRenderTime);
		RenderingEngine::Reset(mRenderTime);

		//Message Loop
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				RenderingEngine::Tick(mRenderTime);

				if (!mPauseApplication)
				{
					FrameStats();
					CameraMovement();
					Update();
					Draw();
				}
			}
		}

		return (int)msg.wParam;
	}
}