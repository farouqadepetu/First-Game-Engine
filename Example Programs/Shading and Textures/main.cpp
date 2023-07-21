#include "Direct3DLink.h"
#include "FADirectXException.h"
#include "WindowProcedure.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	MVC::Model model;
	MVC::View view;

	MVC::Controller controller(&model, &view);

	view.CreateWindows(hInstance, WindowProc::MainWindowProc, WindowProc::RenderingWindowProc, &controller, &controller);

	model.InitializeScene(view.GetRenderingWindow());

	return controller.Run();
}