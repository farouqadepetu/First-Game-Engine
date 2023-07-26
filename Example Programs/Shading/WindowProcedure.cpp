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
				SendMessage(renderingWindow.windowHandle, uMsg, wParam, lParam);
				return 0;
			}

			case WM_SIZE:
			{
				SetWindowPos(renderingWindow.windowHandle, nullptr, 0, 0, RenderingEngine::GetWidth(mainWindow) - dropDownListWidth, 
					RenderingEngine::GetHeight(mainWindow), SWP_NOMOVE | SWP_SHOWWINDOW);

				//Adjust the locations of the drop down lists.
				for (auto& i : dropDownLists)
				{
					SetWindowPos(i.windowHandle, nullptr, RenderingEngine::GetWidth(renderingWindow), 
						RenderingEngine::GetY(i), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}

				//Adjust the location of the static texts.
				for (auto& i : staticText)
				{
					SetWindowPos(i.windowHandle, nullptr, RenderingEngine::GetWidth(renderingWindow),
						RenderingEngine::GetY(i), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}

				//Adjust the locations of the buttons.
				for (auto& i : buttons)
				{
					SetWindowPos(i.windowHandle, nullptr, RenderingEngine::GetWidth(renderingWindow) + 20,
						RenderingEngine::GetY(i), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}

				//Execute the WM_SIZE message in the rendering windows message procedure.
				SendMessage(renderingWindow.windowHandle, uMsg, wParam, lParam);

				return 0;
			}

			case WM_EXITSIZEMOVE:
			{
				SetWindowPos(renderingWindow.windowHandle, nullptr, 0, 0, RenderingEngine::GetWidth(mainWindow) - dropDownListWidth,
					RenderingEngine::GetHeight(mainWindow), SWP_NOMOVE | SWP_SHOWWINDOW);

				//Adjust the locations of the drop down lists.
				for (auto& i : dropDownLists)
				{
					SetWindowPos(i.windowHandle, nullptr, RenderingEngine::GetWidth(renderingWindow),
						RenderingEngine::GetY(i), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}

				//Adjust the location of the static texts.
				for (auto& i : staticText)
				{
					SetWindowPos(i.windowHandle, nullptr, RenderingEngine::GetWidth(renderingWindow),
						RenderingEngine::GetY(i), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}

				//Adjust the locations of the buttons.
				for (auto& i : buttons)
				{
					SetWindowPos(i.windowHandle, nullptr, RenderingEngine::GetWidth(renderingWindow) + 20,
						RenderingEngine::GetY(i), 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
				}

				//Execute the WM_SIZE message in the rendering windows message procedure.
				SendMessage(renderingWindow.windowHandle, uMsg, wParam, lParam);

				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				//Send message to the rendering window procedure.
				SendMessage(renderingWindow.windowHandle, uMsg, wParam, lParam);

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
						camera.position = vec3{ 0.0f, 0.0f, -5.0f };
						camera.x = vec3{ 1.0f, 0.0f, 0.0f };
						camera.y = vec3{ 0.0f, 1.0f, 0.0f };
						camera.z = vec3{ 0.0f, 0.0f, 1.0f };
					}
					else if (LOWORD(wParam) == RESET_SHAPE)
					{
						shapes.at(currentSelection.at(SHAPES))->orientation = MathEngine::Quaternion{};
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
					RenderingEngine::Stop(frameTime);
				}
				else
				{
					isAppPaused = false;
					RenderingEngine::Start(frameTime);

				}
				return 0;
			}

			case WM_SIZE: //when the window gets resized.
			{
				if (shadingScene != nullptr)
				{
					int width = RenderingEngine::GetWidth(renderingWindow);
					int height = RenderingEngine::GetHeight(renderingWindow);

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

						pProjection.aspectRatio = ((float)width / height);

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

							pProjection.aspectRatio = ((float)width / height);

							ResizeText(width, height);
						}
						//restoring from a maximized state
						else if (isMaximized)
						{
							isMaximized = false;

							shadingScene->Resize(width, height, windowHandle, true, true);

							pProjection.aspectRatio = ((float)width / height);

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
				RenderingEngine::Stop(frameTime);
				return 0;
			}

			//when the user releases the resize bars.
			case WM_EXITSIZEMOVE:
			{
				isAppPaused = false;
				RenderingEngine::Start(frameTime);

				if (shadingScene != nullptr)
				{
					int width = RenderingEngine::GetWidth(renderingWindow);
					int height = RenderingEngine::GetHeight(renderingWindow);

					shadingScene->Resize(width, height, windowHandle, true, true);

					pProjection.aspectRatio = ((float)width / height);

					ResizeText(width, height);
				}

				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				if (GET_X_LPARAM(lParam) > RenderingEngine::GetWidth(renderingWindow))
					enableCameraUserInput = false;
				else
					enableCameraUserInput = true;

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

	void ResizeText(unsigned int width, unsigned int height)
	{
		//Resize the FPS text
		framesPerSecond.SetTextLocation(MathEngine::Vector4D{0.0f, 0.0f, 300.0f, 0.0f});
	}
}