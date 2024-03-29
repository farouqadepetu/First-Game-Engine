#include "Direct3DLink.h"
#include "DirectXException.h"
#include "DisplayShapesGlobalVariables.h"
#include "WindowProcedure.h"
#include "InitFunctions.h"
#include "MessageLoopFunctions.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	//Main window
	WNDCLASSEX mainWindowClass{};
	mainWindowClass.cbSize = sizeof(WNDCLASSEX);
	mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	mainWindowClass.lpfnWndProc = WindowProc::DisplayShapesWindowProc;
	mainWindowClass.cbClsExtra = 0;
	mainWindowClass.cbWndExtra = 0;
	mainWindowClass.hInstance = hInstance;
	mainWindowClass.hIcon = nullptr;
	mainWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mainWindowClass.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
	mainWindowClass.lpszMenuName = nullptr;
	mainWindowClass.lpszClassName = L"Main Window Class";
	mainWindowClass.hIconSm = nullptr;

	//The window where the shapes will be rendered to.
	RenderingEngine::Window displayShapesWindow{};
	RenderingEngine::CreateParentWindow(displayShapesWindow, hInstance, WindowProc::DisplayShapesWindowProc, RenderingEngine::Color(0.4f, 0.4f, 0.4f, 1.0f),
		L"Main Window Class", L"Display Shapes Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 720);
	GlobalVariables::window = &displayShapesWindow;

	//The RenderScene we use to render the shapes.
	RenderingEngine::RenderScene displayShapesScene(RenderingEngine::GetWidth(displayShapesWindow), RenderingEngine::GetHeight(displayShapesWindow),
		displayShapesWindow.windowHandle, GlobalVariables::isMSAAEnabled, GlobalVariables::isTextEnabled);
	GlobalVariables::scene = &displayShapesScene;

	//Initialization Functions
	Init::BuildShapes();
	Init::BuildCamera();
	Init::BuildPerspectiveProjection(RenderingEngine::GetWidth(displayShapesWindow), RenderingEngine::GetHeight(displayShapesWindow));
	Init::BuildShaders(displayShapesScene);
	Init::BuildVertexAndIndexList(displayShapesScene);
	Init::BuildVertexAndIndexBuffers(displayShapesScene);
	Init::BuildConstantBuffers(displayShapesScene);
	Init::BuildPSOs(displayShapesScene);
	Init::BuildText(RenderingEngine::GetWidth(displayShapesWindow), RenderingEngine::GetHeight(displayShapesWindow));

	MSG msg{};

	RenderingEngine::InitializeTime(GlobalVariables::frameTime);
	RenderingEngine::Reset(GlobalVariables::frameTime);

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
			RenderingEngine::Tick(GlobalVariables::frameTime);

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