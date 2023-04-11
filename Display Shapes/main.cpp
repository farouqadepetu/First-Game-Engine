#include "Direct3DLink.h"
#include "FADirectXException.h"
#include "DisplayShapesGlobalVariables.h"
#include "WindowProcedure.h"
#include "InitFunctions.h"
#include "MessageLoopFunctions.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	try
	{
		//The window where the shapes will be rendered to.
		FAWindow::Window displayShapesWindow(hInstance, L"Display Shapes Window Class", L"Display Shapes Window",
			WindowProc::DisplayShapesWindowProc, 1024, 720);
		GlobalVariables::window = &displayShapesWindow;

		//The RenderScene we use to render the shapes.
		FARender::RenderScene displayShapesScene(displayShapesWindow.GetWidth(), displayShapesWindow.GetHeight(), 
			displayShapesWindow.GetWindowHandle(), GlobalVariables::isMSAAEnabled, GlobalVariables::isTextEnabled);
		GlobalVariables::scene = &displayShapesScene;

		//Initialization Functions
		Init::BuildShapes();
		Init::BuildCamera(displayShapesWindow.GetWidth(), displayShapesWindow.GetHeight());
		Init::BuildShaders(displayShapesScene);
		Init::BuildVertexAndIndexList(displayShapesScene);
		Init::BuildVertexAndIndexBuffers(displayShapesScene);
		Init::BuildConstantBuffers(displayShapesScene);
		Init::BuildPSOs(displayShapesScene);
		Init::BuildText(displayShapesWindow.GetWidth(), displayShapesWindow.GetHeight());

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
					MessageLoop::Update(displayShapesScene);
					MessageLoop::Draw(displayShapesScene);
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