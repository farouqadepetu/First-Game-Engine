#include "Direct3DLink.h"
#include "FADirectXException.h"
#include "GlobalVariables.h"
#include "WindowProcedure.h"
#include "InitFunctions.h"
#include "MessageLoopFunctions.h"
#include <CommCtrl.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	try
	{
		//Initialization Functions
		Init::BuildMainWindow(hInstance);
		Init::BuildRenderingWindow(hInstance);
		Init::BuildDropDownLists(hInstance);
		Init::BuildButtons(hInstance);
		Init::BuildMaterials();
		Init::BuildLightSources();
		Init::BuildShapes();
		Init::BuildCamera();
		Init::BuildScene();
		Init::BuildShaders();
		Init::BuildVertexAndIndexList();
		Init::BuildVertexAndIndexBuffers();
		Init::BuildConstantBuffers();
		Init::BuildPSOs();
		Init::BuildText();

		MSG msg{};
		GlobalVariables::frameTime.Reset();

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
				GlobalVariables::frameTime.Tick();

				if (!GlobalVariables::isAppPaused)
				{
					MessageLoop::FrameStats();
					MessageLoop::UserInput();
					MessageLoop::Update();
					MessageLoop::Draw();
				}
			}
		}
	}
	catch (std::runtime_error& re)
	{
		std::string eMsg{ re.what() };
		std::wstring errorMsg{ AnsiToWString(eMsg) };
		MessageBox(nullptr, errorMsg.c_str(), L"Run Time Error", MB_OK);
	}
	catch (std::out_of_range& outOfRange)
	{
		std::string eMsg{ outOfRange.what() };
		std::wstring errorMsg{ AnsiToWString(eMsg) };
		MessageBox(nullptr, errorMsg.c_str(), L"Out of Range Error", MB_OK);
	}
	catch (DirectXException& dx)
	{
		MessageBox(nullptr, dx.ErrorMsg().c_str(), L"DirectX Error", MB_OK);
	}
}