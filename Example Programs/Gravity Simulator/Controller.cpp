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
				con->mRenderTime.Stop();
			}
			else
			{
				con->mPauseApplication = false;
				con->mRenderTime.Start();
			}

			return 0;
		}

		case WM_SIZE:
		{
			unsigned int newWidth = LOWORD(lParam);
			unsigned int newHeight = HIWORD(lParam);
			con->mView.ResizeRenderingWindow(newWidth, newHeight);
			con->mPerspectiveProjection.SetAspectRatio((float)newWidth / newHeight);

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

				con->mRenderTime.Stop();

			}
			else if (wParam == SIZE_RESTORED)
			{
				if (con->mView.IsRenderingWindowMinimized())
				{
					con->mView.UnMinimizeRenderingWindow();

					con->mScene->Resize(newWidth, newHeight, windowHandle);

					con->mRenderTime.Start();
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
			con->mRenderTime.Stop();
			con->mView.ResizingRenderingWindow();
			return 0;
		}

		case WM_EXITSIZEMOVE:
		{
			con->mPauseApplication = false;
			con->mRenderTime.Start();
			con->mView.NotResizingRenderingWindow();

			RECT renderingWindowClientAreaRect{};

			GetClientRect(windowHandle, &renderingWindowClientAreaRect);

			unsigned int newWidth = renderingWindowClientAreaRect.right;
			unsigned int newHeight = renderingWindowClientAreaRect.bottom;

			con->mView.ResizeRenderingWindow(newWidth, newHeight);

			con->mScene->Resize(newWidth, newHeight, windowHandle);

			con->mPerspectiveProjection.SetAspectRatio((float)newWidth / newHeight);

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

			con->mView.DisplaySettingsWindow();

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
		mScene = std::make_unique<FARender::RenderScene>(mView.GetRenderingWindowWidth(), mView.GetRenderingWindowHeight(),
			mView.GetRenderingWindowHandle());

		LoadShaders();
		CreateRootSignature();
		CreatePSO();
		CreateVertexAndIndexBuffers();
		CreateConstantBuffer();

		mScene->ExecuteAndFlush();

		mCamera.SetProperties(vec4(10.0f, 0.0f, -25.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 0.0f),
			5.0f, 0.125f);

		mPerspectiveProjection.SetProperties(1.0f, 1000.0f, 45.0f, (float)mView.GetRenderingWindowWidth() / mView.GetRenderingWindowHeight());
	}

	void Controller::LoadShaders()
	{
		mScene->CompileShader(L"GS Vertex Shader", L"GravitySimulatorVS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(L"GS Pixel Shader", L"GravitySimulatorPS.hlsl", "psMain", "ps_5_1");

		mScene->CreateInputElementDescription(L"GS VS Input", "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		mScene->CreateInputElementDescription(L"GS VS Input", "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16);
		mScene->CreateInputElementDescription(L"GS VS Input", "TEXCOORDS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32);

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
		mScene->CreateDynamicBuffer(L"Object Constant Buffer", 7 * sizeof(ObjectConstants), nullptr, sizeof(ObjectConstants));
	}

	void Controller::CameraMovement()
	{
		mCamera.KeyboardInputWASD(mRenderTime.GetDeltaTime());
		mCamera.MouseInput();
	}

	void Controller::Update()
	{
		mCamera.UpdateViewMatrix();
		mPerspectiveProjection.UpdateProjectionMatrix();

		if(mPlayAnimaton)
			mModel.Simulate(mRenderTime.GetDeltaTime());

		mModel.UpdateModels(mScene.get(), mCamera.GetViewMatrix(), mPerspectiveProjection.GetProjectionMatrix());
	}

	void Controller::Draw()
	{
		mScene->BeforeRenderObjects();

		mScene->LinkPSOAndRootSignature(L"GS PSO WIREFRAME", L"GS Root Sig");

		mScene->LinkStaticBuffer(FARender::VERTEX_BUFFER, L"Vertex Buffer");
		mScene->LinkStaticBuffer(FARender::INDEX_BUFFER, L"Index Buffer");

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

		timeElapsed += mRenderTime.GetDeltaTime();
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

	void Controller::LerpCamera()
	{
		static float t = 0.0f;
		static float lerpSpeed = 1.0f / 1.5f;
		static vec4 start = mCamera.GetCameraPosition();
		static vec4 end = mCamera.GetCameraPosition() + vec4(0.0f, 5.0f, 0.0f, 0.0f);
		mCamera.SetCameraPosition(FAMath::Lerp(start, end, t));

		t += mRenderTime.GetDeltaTime() * lerpSpeed;

		if (t >= 1.0f)
		{
			vec4 temp = start;
			start = end;
			end = temp;
			t = 0.0f;
		}
	}

	void Controller::SlerpCamera()
	{
		static float t = 0.0f;
		static float lerpSpeed = 1.0f / 1.0f;

		static vec4 startPos(mCamera.GetCameraPosition());
		static vec4 n(mCamera.GetZ());
		static vec4 v(mCamera.GetY());
		static vec4 u(mCamera.GetX());

		static FAMath::Quaternion start(1.0f, 0.0f, 0.0f, 0.0f);
		static FAMath::Quaternion start2(1.0f, 0.0f, 0.0f, 0.0f);
		static FAMath::Quaternion end(FAMath::RotationQuaternion(180.0f, vec3(1.0f, 0.0f, 0.0f)));
		static FAMath::Quaternion end2(FAMath::RotationQuaternion(180.0f, mCamera.GetX()));

		static FAMath::Quaternion posQ;
		static FAMath::Quaternion axisQ;

		t += mRenderTime.GetDeltaTime() * lerpSpeed;

		if (t >= 1.0f)
		{
			/*startPos = FAMath::Rotate(cPos, startPos);
			u = FAMath::Rotate(axisQ, u);
			v = FAMath::Rotate(axisQ, v);
			n = FAMath::Rotate(axisQ, n);*/

			FAMath::Quaternion temp(start);
			start = end;
			end = temp;

			temp = start2;
			start2 = end2;
			end2 = temp;

			t = 0.0f;
		}

		posQ = FAMath::Slerp(start, end, t);
		axisQ = FAMath::Slerp(start2, end2, t);
		
		mCamera.SetCameraPosition(FAMath::Rotate(posQ, startPos));
		mCamera.SetX(FAMath::Rotate(axisQ, u));
		mCamera.SetY(FAMath::Rotate(axisQ, v));
		mCamera.SetZ(FAMath::Rotate(axisQ, n));

		if (mCamera.GetCameraPosition() == startPos)
		{
			end = FAMath::RotationQuaternion(80.0f, vec3(0.0f, -1.0f, 0.0f));
			end2 = FAMath::RotationQuaternion(80.0f, -mCamera.GetY());
		}
	}

	int Controller::Run()
	{
		MSG msg{};

		mRenderTime.Reset();

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
				mRenderTime.Tick();
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