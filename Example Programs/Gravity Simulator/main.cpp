#include "Direct3DLink.h"
#include "DirectXException.h"
#include "Controller.h"
#include <iomanip>


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	MVC::Controller controller(hInstance);

	return controller.Run();
}