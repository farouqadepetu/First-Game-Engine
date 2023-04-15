#include "WindowProcedure.h"
#include "GlobalVariables.h"

using namespace GlobalVariables;

namespace WindowProc
{
	//Window Procedure
	LRESULT DisplayShapesWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				if (scene != nullptr && window != nullptr)
				{
					window->SetWidth(LOWORD(lParam));
					window->SetHeight(HIWORD(lParam));
					
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
						scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
							true, true);

						camera.SetAspectRatio((float)window->GetWidth() / window->GetHeight());

						ResizeText(window->GetWidth(), window->GetHeight());
					}
					if (wParam == SIZE_RESTORED)
					{
						//restoring from a minimized state
						if (isMinimized)
						{
							isAppPaused = false;
							isMinimized = false;
							scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
								true, true);

							camera.SetAspectRatio((float)window->GetWidth() / window->GetHeight());

							ResizeText(window->GetWidth(), window->GetHeight());
						}
						//restoring from a maximized state
						else if (isMaximized)
						{
							isMaximized = false;
							scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
								true, true);

							camera.SetAspectRatio((float)window->GetWidth() / window->GetHeight());

							ResizeText(window->GetWidth(), window->GetHeight());
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

				RECT windowSize{};
				GetWindowRect(windowHandle, &windowSize);

				window->SetWidth(windowSize.right - windowSize.left);
				window->SetHeight(windowSize.bottom - windowSize.top);

				scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
					true, true);

				camera.SetAspectRatio((float)window->GetWidth() / window->GetHeight());

				ResizeText(window->GetWidth(), window->GetHeight());

				return 0;
			}

			case WM_KEYDOWN: //if they user every presses a char key.
			{
				switch (wParam)
				{
					case VK_DOWN:
						if (currentArrow < numSelections)
						{
							++currentArrow;
							FAMath::Vector4D saLocation(selectionArrow.GetTextLocation());
							saLocation.SetY(saLocation.GetY() + heightIncrease);
							saLocation.SetW(saLocation.GetW() + heightIncrease);
							selectionArrow.SetTextLocation(saLocation);
						}
						break;
					
					case VK_UP:
						if (currentArrow > 0)
						{
							--currentArrow;
							FAMath::Vector4D saLocation(selectionArrow.GetTextLocation());
							saLocation.SetY(saLocation.GetY() - heightIncrease);
							saLocation.SetW(saLocation.GetW() - heightIncrease);
							selectionArrow.SetTextLocation(saLocation);
						}
						break;

					case VK_LEFT:
						if (currentArrow == SHADING)
						{
							if (currentSelection.at(SHADING) > 1)
								--currentSelection.at(SHADING);
						}
						else if (currentArrow == SHAPES)
						{
							if (currentSelection.at(SHAPES) > 1)
								--currentSelection.at(SHAPES);
						}
						else if (currentArrow == MATERIALS)
						{
							if (currentSelection.at(MATERIALS) > 1)
								--currentSelection.at(MATERIALS);
						}
						else if (currentArrow == LIGHT_SOURCE)
						{
							if (currentSelection.at(LIGHT_SOURCE) > 1)
								--currentSelection.at(LIGHT_SOURCE);
						}
						break;

					case VK_RIGHT:
						if (currentArrow == SHADING)
						{
							if (currentSelection.at(SHADING) < numShading)
								++currentSelection.at(SHADING);
						}
						else if (currentArrow == SHAPES)
						{
							if (currentSelection.at(SHAPES) < numShapes)
								++currentSelection.at(SHAPES);
						}
						else if (currentArrow == MATERIALS)
						{
							if (currentSelection.at(MATERIALS) < numMaterials)
								++currentSelection.at(MATERIALS);
						}
						else if (currentArrow == LIGHT_SOURCE)
						{
							if (currentSelection.at(LIGHT_SOURCE) < numLightSources)
								++currentSelection.at(LIGHT_SOURCE);
						}
						break;
				}
				return 0;
			}

			case WM_CHAR:
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
		float selectionTextX = width - (21.0f * textSize);
		float arrowX{ selectionTextX - 20.0f  };
		float halfTextSize{ textSize / 2.0f };
		unsigned int h = 0.0f;

		//Resize the FPS text
		framesPerSecond.SetTextLocation(FAMath::Vector4D(0.0f, 0.0f, 20.0f * textSize, 0.0f));

		//Resize the selection arrow
		selectionArrow.SetTextLocation(FAMath::Vector4D(arrowX, currentArrow * heightIncrease,
			arrowX - 2.0f, currentArrow * (heightIncrease + heightIncrease)));

		//Resize the selections text
		for (unsigned int i = 0; i <= numSelections; ++i)
		{
			selections.at(i).at(0).SetTextLocation(FAMath::Vector4D(selectionTextX, h ,
				selectionTextX + (selections.at(i).at(0).GetTextSize() * halfTextSize), h + heightIncrease));

			h += heightIncrease;
		}

		//Resize the options text
		h = 0.0f;
		for (unsigned int i = 0; i <= numSelections; ++i)
		{
			unsigned int numCols = (unsigned int)selections.at(i).size();
			for (unsigned int j = 1; j < numCols; ++j)
			{
				selections.at(i).at(j).SetTextLocation(
					FAMath::Vector4D(selectionTextX + (selections.at(i).at(0).GetTextSize() * halfTextSize),
					h, width, h + heightIncrease));
			}

			h += heightIncrease;
		}
	}

	void Reset(unsigned int width, unsigned int height)
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
	}
}