
#include "WindowProcedure.h"
#include "Controller.h"
#include <Windowsx.h>
#include <CommCtrl.h>

namespace WindowProc
{
	LRESULT MainWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				if (con->GetView()->GetRenderingWindow().GetWindowHandle() != nullptr)
				{
					//Send message to the rendering window procedure.
					SendMessage(con->GetView()->GetRenderingWindow().GetWindowHandle(), uMsg, wParam, lParam);
				}
				
				return 0;
			}

			case WM_SIZE:
			{
				if (con->GetView()->GetRenderingWindow().GetWindowHandle() != nullptr)
				{
					RECT mainWindowClientRect{};

					//Get the new width and height of the client area of the main window
					GetClientRect(windowHandle, &mainWindowClientRect);

					con->GetView()->ResizeRenderingWindow(mainWindowClientRect.right - 200, mainWindowClientRect.bottom);

					con->GetView()->ResizeControlWindows();

					//Execute the WM_SIZE message in the rendering windows message procedure.
					SendMessage(con->GetView()->GetRenderingWindow().GetWindowHandle(), uMsg, wParam, lParam);
				}

				return 0;
			}

			case WM_EXITSIZEMOVE:
			{
				if (con->GetView()->GetRenderingWindow().GetWindowHandle() != nullptr)
				{
					RECT mainWindowClientRect{};

					//Get the new width and height of the client area of the main window
					GetClientRect(windowHandle, &mainWindowClientRect);

					con->GetView()->ResizeRenderingWindow(mainWindowClientRect.right - 200, mainWindowClientRect.bottom);

					con->GetView()->ResizeControlWindows();

					//Execute the WM_SIZE message in the rendering windows message procedure.
					SendMessage(con->GetView()->GetRenderingWindow().GetWindowHandle(), uMsg, wParam, lParam);
				}

				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				//Send message to the rendering window procedure.
				SendMessage(con->GetView()->GetRenderingWindow().GetWindowHandle(), uMsg, wParam, lParam);
				return 0;
			}

			case WM_COMMAND:
			{
				//if the user activates/deactivates the drop down list
				if (HIWORD(wParam) == CBN_DROPDOWN || HIWORD(wParam) == CBN_CLOSEUP)
				{
					con->GetView()->DisableCameraMovement();

					SetFocus(windowHandle);
				}

				else if (HIWORD(wParam) == CBN_SELENDOK) //If the user changed their selection
				{
					unsigned int index = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

					if (LOWORD(wParam) == MVC::SHAPES)
						con->GetView()->SetCurrentShape(index);

					else if (LOWORD(wParam) == MVC::TEXTURES)
						con->GetView()->SetCurrentTexture(index);

					else if (LOWORD(wParam) == MVC::RENDER_OPTIONS)
						con->GetView()->SetCurrentRenderOption(index);

					else if (LOWORD(wParam) == MVC::LIGHT_SOURCE)
						con->GetView()->SetCurrentLightSource(index);

					con->GetView()->DisableCameraMovement();
					SetFocus(windowHandle);
				}

				else if (HIWORD(wParam) == BN_CLICKED)
				{
					if (LOWORD(wParam) == MVC::PLAY_PAUSE_SHAPE)
					{
						(con->GetView()->GetRotateShape()) ? con->GetView()->RotateShape(false) : con->GetView()->RotateShape(true);
					}
					else if (LOWORD(wParam) == MVC::PLAY_PAUSE_POINT_LIGHT)
					{
						(con->GetView()->GetRotatePointLight()) ? 
							con->GetView()->RotatePointLight(false) : con->GetView()->RotatePointLight(true);
					}
					else if (LOWORD(wParam) == MVC::RESET_CAMERA)
					{
						con->GetModel()->ResetCamera();
					}
					else if (LOWORD(wParam) == MVC::RESET_SHAPE)
					{
						con->GetModel()->ResetShape(con->GetView()->GetCurrentShape());

					}
					else if (LOWORD(wParam) == MVC::RESET_POINT_LIGHT)
					{
						con->GetModel()->ResetPointLights();
					}

					con->GetView()->DisableCameraMovement();
					SetFocus(windowHandle);
				}

				return 0;
			}

			//Changes the fore and background color of static text
			case WM_CTLCOLORSTATIC:
			{
				HDC hdcStatic = (HDC)wParam;
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkColor(hdcStatic, RGB(100, 100, 100));

				return (LRESULT)GetStockObject(NULL_BRUSH);
			}


			case WM_DESTROY: //when the user exits the window.
			{
				PostQuitMessage(0);
				return 0;
			}
		}

		//handles events that weren't manually handled.
		return DefWindowProc(windowHandle, uMsg, wParam, lParam);
	}

	//Window Procedure
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
		case WM_ACTIVATE: //handles the events where the window gets activated/deactivated.
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				con->GetView()->DeactivateMainWindow();
				con->GetModel()->GetFrameTime().Stop();
			}
			else
			{
				con->GetView()->ActivateMainWindow();
				con->GetModel()->GetFrameTime().Start();

			}
			return 0;
		}

		case WM_SIZE: //when the window gets resized.
		{
			if (con->GetModel()->GetScene() != nullptr)
			{
				int width = con->GetView()->GetRenderingWindow().GetWidth();
				int height = con->GetView()->GetRenderingWindow().GetHeight();

				if (wParam == SIZE_MINIMIZED) //window gets minimized
				{
					con->GetView()->DeactivateMainWindow();
					con->GetView()->MinimizeMainWindow();
					con->GetView()->UnMaximizeMainWindow();
				}
				if (wParam == SIZE_MAXIMIZED) //window gets maximized
				{
					con->GetView()->ActivateMainWindow();
					con->GetView()->UnMinimizeMainWindow();
					con->GetView()->MaximizeMainWindow();

					con->GetModel()->GetScene()->Resize(width, height, windowHandle, true, true);

					con->GetModel()->GetPerspectiveProjection().SetAspectRatio((float)width / height);
				}
				if (wParam == SIZE_RESTORED)
				{
					//restoring from a minimized state
					if (con->GetView()->GetIsMainWindowMinimized())
					{
						con->GetView()->ActivateMainWindow();
						con->GetView()->UnMinimizeMainWindow();

						con->GetModel()->GetScene()->Resize(width, height, windowHandle, true, true);

						con->GetModel()->GetPerspectiveProjection().SetAspectRatio((float)width / height);
					}
					//restoring from a maximized state
					else if (con->GetView()->GetIsMainWindowMaximized())
					{
						con->GetView()->UnMaximizeMainWindow();

						con->GetModel()->GetScene()->Resize(width, height, windowHandle, true, true);

						con->GetModel()->GetPerspectiveProjection().SetAspectRatio((float)width / height);
					}
				}
			}

			return 0;
		}

		//when user grabs resize bars.
		case WM_ENTERSIZEMOVE:
		{
			con->GetView()->DeactivateMainWindow();
			con->GetModel()->GetFrameTime().Stop();
			return 0;
		}

		//when the user releases the resize bars.
		case WM_EXITSIZEMOVE:
		{
			con->GetView()->ActivateMainWindow();
			con->GetModel()->GetFrameTime().Start();

			if (con->GetModel()->GetScene() != nullptr)
			{
				int width = con->GetView()->GetRenderingWindow().GetWidth();
				int height = con->GetView()->GetRenderingWindow().GetHeight();

				con->GetModel()->GetScene()->Resize(width, height, windowHandle, true, true);

				con->GetModel()->GetPerspectiveProjection().SetAspectRatio((float)width / height);
			}

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			if (GET_X_LPARAM(lParam) > con->GetView()->GetRenderingWindow().GetWidth())
				con->GetView()->DisableCameraMovement();
			else
				con->GetView()->EnableCameraMovement();

			return 0;
		}


		case WM_DESTROY: //when the user exits the window.
		{
			PostQuitMessage(0);
			return 0;
		}
		}

		return DefWindowProc(windowHandle, uMsg, wParam, lParam); //handles events that weren't manually handled.
	}

}