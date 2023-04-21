#include "WindowProcedure.h"
#include "GlobalVariables.h"
#include <Windowsx.h>

using namespace GlobalVariables;

namespace WindowProc
{
	LRESULT MainWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_ACTIVATE:
			{
				//Send message to the rendering window procedure.
				SendMessage(renderingWindow.GetWindowHandle(), uMsg, wParam, lParam);
				return 0;
			}

			case WM_SIZE:
			{
				//Get the new width and height of the client area of the main window
				GetClientRect(windowHandle, &mainWindowClientRect);

				//Adjust width and height of the rendering window based of the width and height of the main window
				renderingWindow.SetWidth(mainWindowClientRect.right - dropDownListWidth);
				renderingWindow.SetHeight(mainWindowClientRect.bottom);
				MoveWindow(renderingWindow.GetWindowHandle(), 0, 0, renderingWindow.GetWidth(), renderingWindow.GetHeight(), FALSE);

				//Adjust the locations of the drop down lists.
				for (auto& i : dropDownLists)
				{
					i.SetX(renderingWindow.GetWidth());
					MoveWindow(i.GetWindowHandle(), i.GetX(), i.GetY(), i.GetWidth(), i.GetHeight(), FALSE);
				}

				//Adjust the location of the static texts.
				for (auto& i : staticText)
				{
					i.SetX(renderingWindow.GetWidth());
					MoveWindow(i.GetWindowHandle(), i.GetX(), i.GetY(), i.GetWidth(), i.GetHeight(), TRUE);
				}

				//Adjust the locations of the buttons.
				for (auto& i : buttons)
				{
					i.SetX(renderingWindow.GetWidth() + 20);
					MoveWindow(i.GetWindowHandle(), i.GetX(), i.GetY(), i.GetWidth(), i.GetHeight(), TRUE);
				}

				//Execute the WM_SIZE message in the rendering windows message procedure.
				SendMessage(renderingWindow.GetWindowHandle(), uMsg, wParam, lParam);

				return 0;
			}

			case WM_EXITSIZEMOVE:
			{
				//Get the new width and height of the client area of the main window
				GetClientRect(windowHandle, &mainWindowClientRect);

				//Adjust width and height of the rendering window based of the width and height of the main window
				renderingWindow.SetWidth(mainWindowClientRect.right - dropDownListWidth);
				renderingWindow.SetHeight(mainWindowClientRect.bottom);

				//Adjust the locations of the drop down lists.
				for (auto& i : dropDownLists)
				{
					i.SetX(renderingWindow.GetWidth());
					MoveWindow(i.GetWindowHandle(), i.GetX(), i.GetY(), i.GetWidth(), i.GetHeight(), FALSE);
				}

				//Adjust the location of the static texts.
				for (auto& i : staticText)
				{
					i.SetX(renderingWindow.GetWidth());
					MoveWindow(i.GetWindowHandle(), i.GetX(), i.GetY(), i.GetWidth(), i.GetHeight(), FALSE);
				}

				//Adjust the locations of the buttons.
				for (auto& i : buttons)
				{
					i.SetX(renderingWindow.GetWidth() + 20);
					MoveWindow(i.GetWindowHandle(), i.GetX(), i.GetY(), i.GetWidth(), i.GetHeight(), TRUE);
				}

				//Execute the WM_SIZE message in the rendering windows message procedure.
				SendMessage(renderingWindow.GetWindowHandle(), uMsg, wParam, lParam);

				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				//Send message to the rendering window procedure.
				SendMessage(renderingWindow.GetWindowHandle(), uMsg, wParam, lParam);

				return 0;
			}

			case WM_KEYDOWN:
			{
				//Send message to the rendering window procedure.
				SendMessage(renderingWindow.GetWindowHandle(), uMsg, wParam, lParam);
				return 0;
			}

			case WM_COMMAND:
			{
				//if the user activates/deactivates the drop down list
				if (HIWORD(wParam) == CBN_DROPDOWN || HIWORD(wParam) == CBN_CLOSEUP)
				{
					enableCameraUserInput = false;

					SetFocus(windowHandle);
				}

				if(HIWORD(wParam) == CBN_SELENDOK) //If the user changed their selection
				{
					int index = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM) 0);

					currentSelection.at(LOWORD(wParam)) = index;

					SetFocus(windowHandle);

				}

				if (HIWORD(wParam) == BN_CLICKED)
				{
					if (LOWORD(wParam) == PLAY_PAUSE)
					{
						playAnimation = (playAnimation) ? false : true;
					}
					else if (LOWORD(wParam) == RESET_CAMERA)
					{
						camera.SetCameraPosition(vec3(0.0f, 0.0f, -12.5f));
						camera.SetX(vec3(1.0f, 0.0f, 0.0f));
						camera.SetY(vec3(0.0f, 1.0f, 0.0f));
						camera.SetZ(vec3(0.0f, 0.0f, 1.0f));
					}
					else if (LOWORD(wParam) == RESET_SHAPE)
					{
						shapes.at(currentSelection.at(SHAPES))->SetXAxis(1.0f, 0.0f, 0.0f);
						shapes.at(currentSelection.at(SHAPES))->SetYAxis(0.0f, 1.0f, 0.0f);
						shapes.at(currentSelection.at(SHAPES))->SetZAxis(0.0f, 0.0f, 1.0f);
					}

					enableCameraUserInput = false;
					SetFocus(windowHandle);
				}

				return 0;
			}

			//Chnages the fore and background color of the text
			case WM_CTLCOLORSTATIC:
			{
				HDC hdcStatic = (HDC)wParam;
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkColor(hdcStatic, RGB(100, 100, 100));

				return (LRESULT)GetStockObject(NULL_BRUSH);
			}

			case WM_DESTROY: //when the user exits the window.
			{
				DeleteObject(textFont);
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
		switch (uMsg)
		{
			case WM_ACTIVATE: //handles the events where the window gets activated/deactivated.
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					isAppPaused = true;
					frameTime.Stop();
				}
				else
				{
					isAppPaused = false;
					frameTime.Start();

				}
				return 0;
			}

			case WM_SIZE: //when the window gets resized.
			{
				if (shadingScene != nullptr)
				{
					int width = renderingWindow.GetWidth();
					int height = renderingWindow.GetHeight();

					if (wParam == SIZE_MINIMIZED) //window gets minimized
					{
						isAppPaused = true;
						isMinimized = true;
						isMaximized = false;
					}
					if (wParam == SIZE_MAXIMIZED) //window gets maximized
					{
						isAppPaused = false;
						isMinimized = false;
						isMaximized = true;

						shadingScene->Resize(width, height, windowHandle,true, true);

						camera.SetAspectRatio((float)width / height);

						ResizeText(width, height);
					}
					if (wParam == SIZE_RESTORED)
					{
						//restoring from a minimized state
						if (isMinimized)
						{
							isAppPaused = false;
							isMinimized = false;

							shadingScene->Resize(width, height, windowHandle, true, true);

							camera.SetAspectRatio((float)width / height);

							ResizeText(width, height);
						}
						//restoring from a maximized state
						else if (isMaximized)
						{
							isMaximized = false;

							shadingScene->Resize(width, height, windowHandle, true, true);

							camera.SetAspectRatio((float)width / height);

							ResizeText(width, height);
						}
					}
				}

				return 0;
			}

			//when user grabs resize bars.
			case WM_ENTERSIZEMOVE:
			{
				isAppPaused = true;
				frameTime.Stop();
				return 0;
			}

			//when the user releases the resize bars.
			case WM_EXITSIZEMOVE:
			{
				isAppPaused = false;
				frameTime.Start();

				if (shadingScene != nullptr)
				{
					int width = renderingWindow.GetWidth();
					int height = renderingWindow.GetHeight();

					shadingScene->Resize(width, height, windowHandle, true, true);

					camera.SetAspectRatio((float)width / height);

					ResizeText(width, height);
				}

				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				if (GET_X_LPARAM(lParam) > renderingWindow.GetWidth())
					enableCameraUserInput = false;
				else
					enableCameraUserInput = true;

				return 0;
			}

			/*case WM_CHAR:
			{
				switch (wParam)
				{
					case 'R':
						Reset(window->GetWidth(), window->GetHeight());
						break;

					case 'r':
						Reset(window->GetWidth(), window->GetHeight());
						break;

					case 'P':
						playAnimation = (playAnimation) ? false : true;
						break;

					case 'p':
						playAnimation = (playAnimation) ? false : true;
						break;
				}

				return 0;
			}*/

			case WM_DESTROY: //when the user exits the window.
			{
				PostQuitMessage(0);
				return 0;
			}
		}

		return DefWindowProc(windowHandle, uMsg, wParam, lParam); //handles events that weren't manually handled.
	}

	void ResizeText(unsigned int width, unsigned int height)
	{
		//Resize the FPS text
		framesPerSecond.SetTextLocation(FAMath::Vector4D(0.0f, 0.0f, 300.0f, 0.0f));
	}

	/*void Reset(unsigned int width, unsigned int height)
	{
		//reset selection arrow to the top
		currentArrow = 0;

		//Reset all selections to their first option/
		for (auto& i : currentSelection)
		{
			i = 1;
		}

		//Resets the orientation of each shape to its starting orientation.
		for (auto& i : shapes)
		{
			i->SetXAxis(1.0f, 0.0f, 0.0f);
			i->SetYAxis(0.0f, 1.0f, 0.0f);
			i->SetZAxis(0.0f, 0.0f, 1.0f);
		}

		//to change the location of the selection arrow.
		ResizeText(width, height);
	}*/
}