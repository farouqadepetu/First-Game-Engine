#include "InitFunctions.h"
#include "WindowProcedure.h"
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <CommCtrl.h>

using namespace GlobalVariables;

namespace Init
{
	void BuildMainWindow(HINSTANCE hInstance)
	{
		//Main window
		WNDCLASSEX mainWindowClass{};
		mainWindowClass.cbSize = sizeof(WNDCLASSEX);
		mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		mainWindowClass.lpfnWndProc = WindowProc::MainWindowProc;
		mainWindowClass.cbClsExtra = 0;
		mainWindowClass.cbWndExtra = 0;
		mainWindowClass.hInstance = hInstance;
		mainWindowClass.hIcon = nullptr;
		mainWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		mainWindowClass.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
		mainWindowClass.lpszMenuName = nullptr;
		mainWindowClass.lpszClassName = L"Main Window Class";
		mainWindowClass.hIconSm = nullptr;

		mainWindow = FAWindow::Window(hInstance, mainWindowClass, L"Shading Window",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720);

	}

	void BuildRenderingWindow(HINSTANCE hInstance)
	{
		//Rendering window
		WNDCLASSEX renderingWindowClass{};
		renderingWindowClass.cbSize = sizeof(WNDCLASSEX);
		renderingWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		renderingWindowClass.lpfnWndProc = WindowProc::RenderingWindowProc;
		renderingWindowClass.cbClsExtra = 0;
		renderingWindowClass.cbWndExtra = 0;
		renderingWindowClass.hInstance = hInstance;
		renderingWindowClass.hIcon = nullptr;
		renderingWindowClass.hCursor = nullptr;
		renderingWindowClass.hbrBackground = nullptr;
		renderingWindowClass.lpszMenuName = nullptr;
		renderingWindowClass.lpszClassName = L"Rendering Window Class";
		renderingWindowClass.hIconSm = nullptr;

		renderingWindow = FAWindow::Window(hInstance, mainWindow.GetWindowHandle(), 0,
			renderingWindowClass, L"Rendering Window",
			WS_CHILD | WS_VISIBLE, 0, 0, mainWindow.GetWidth() - dropDownListWidth, mainWindow.GetHeight());
	}

	void BuildDropDownLists(HINSTANCE hInstance)
	{
		unsigned int dropDownListY = 50;
		unsigned int staticTextY = 25;
		for (int i = 0; i < 4; ++i)
		{
			//Create drop down list
			dropDownLists.emplace_back(hInstance, mainWindow.GetWindowHandle(), i, WC_COMBOBOX, L"",
				CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
				renderingWindow.GetWidth(), dropDownListY, dropDownListWidth, dropDownListHeight);

			//Create static text for the drop down list
			staticText.emplace_back(hInstance, mainWindow.GetWindowHandle(), i, L"STATIC", L"",
				WS_VISIBLE | WS_CHILD | SS_CENTER, renderingWindow.GetWidth(), staticTextY, dropDownListWidth, dropDownListHeight);

			//Set the font of the static text
			HDC hdc = GetDC(staticText.at(i).GetWindowHandle());
			LOGFONT logFont = { 0 };
			logFont.lfHeight = -MulDiv(15, GetDeviceCaps(hdc, LOGPIXELSY), 72);
			logFont.lfWeight = FW_BOLD;
			textFont = CreateFontIndirect(&logFont);
			SendMessage(staticText.at(i).GetWindowHandle(), WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);
			ReleaseDC(staticText.at(i).GetWindowHandle(), hdc);

			dropDownListY += dropDownListHeight;
			staticTextY += dropDownListHeight;

			currentSelection.push_back(0);
		}

		SetWindowText(staticText.at(SHADING).GetWindowHandle(), L"SHADING");
		SetWindowText(staticText.at(SHAPES).GetWindowHandle(), L"SHAPES");
		SetWindowText(staticText.at(MATERIALS).GetWindowHandle(), L"MATERIALS");
		SetWindowText(staticText.at(LIGHT_SOURCE).GetWindowHandle(), L"LIGHT SOURCE");


		//Add the options to the shading drop down list
		SendMessage(dropDownLists.at(SHADING).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Gouraud");
		SendMessage(dropDownLists.at(SHADING).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Phong");
		SendMessage(dropDownLists.at(SHADING).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Blinn-Phong");
		SendMessage(dropDownLists.at(SHADING).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Flat-Phong");
		SendMessage(dropDownLists.at(SHADING).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Flat-Blinn");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists.at(SHADING).GetWindowHandle(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//Add the options to the shape drop down list
		SendMessage(dropDownLists.at(SHAPES).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Box");
		SendMessage(dropDownLists.at(SHAPES).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Pyramid");
		SendMessage(dropDownLists.at(SHAPES).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Cylinder");
		SendMessage(dropDownLists.at(SHAPES).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Cone");
		SendMessage(dropDownLists.at(SHAPES).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Sphere");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists.at(SHAPES).GetWindowHandle(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//Add the options to the shape drop down list
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Bronze");
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Polished Bronze");
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Silver");
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Polished Silver");
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Gold");
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Polished Gold");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists.at(MATERIALS).GetWindowHandle(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//Add the options to the shape drop down list
		SendMessage(dropDownLists.at(LIGHT_SOURCE).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Point");
		SendMessage(dropDownLists.at(LIGHT_SOURCE).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Directional");
		SendMessage(dropDownLists.at(LIGHT_SOURCE).GetWindowHandle(), CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Point + Directional");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists.at(LIGHT_SOURCE).GetWindowHandle(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}

	void BuildButtons(HINSTANCE hInstance)
	{
		unsigned int buttonY = dropDownLists.at(dropDownLists.size() - 1).GetY() + 90;
		unsigned int buttonX = renderingWindow.GetWidth() + 20;

		buttons.emplace_back(hInstance, mainWindow.GetWindowHandle(), 0, L"BUTTON", L"PLAY/PAUSE ROTATION",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_MULTILINE,
			buttonX, buttonY, buttonWidth, buttonHeight);

		buttonY += buttonHeight + 10;
		buttons.emplace_back(hInstance, mainWindow.GetWindowHandle(), 1, L"BUTTON", L"RESET CAMERA",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_MULTILINE,
			buttonX, buttonY, buttonWidth, buttonHeight);

		buttonY += buttonHeight + 10;
		buttons.emplace_back(hInstance, mainWindow.GetWindowHandle(), 2, L"BUTTON", L"RESET SHAPE",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_MULTILINE,
			buttonX, buttonY, buttonWidth, buttonHeight);
	}

	void BuildMaterials()
	{
		//BRONZE
		Material bronze;
		bronze.ambient = FAColor::Color(0.2125f, 0.1275f, 0.054f, 1.0f);
		bronze.diffuse = FAColor::Color(0.714f, 0.4284f, 0.18144f, 1.0f);
		bronze.specular = FAColor::Color(0.393548f, 0.271906f, 0.166721f, 1.0f);
		bronze.shininess = 25.6f;
		materials.emplace_back(bronze);

		//POLISHED BRONZE
		Material polishedBronze;
		polishedBronze.ambient = FAColor::Color(0.25f, 0.148f, 0.06475f, 1.0f);
		polishedBronze.diffuse = FAColor::Color(0.4f, 0.2368f, 0.1036f, 1.0f);
		polishedBronze.specular = FAColor::Color(0.774597f, 0.458561f, 0.200621f, 1.0f);
		polishedBronze.shininess = 76.8f;
		materials.emplace_back(polishedBronze);

		//SILVER
		Material silver;
		silver.ambient = FAColor::Color(0.19225f, 0.19225f, 0.19225f, 1.0f);
		silver.diffuse = FAColor::Color(0.50754f, 0.50754f, 0.50754f, 1.0f);
		silver.specular = FAColor::Color(0.508273f, 0.508273f, 0.508273f, 1.0f);
		silver.shininess = 51.2f;
		materials.emplace_back(silver);

		//POLISHED SILVER
		Material polishedSilver;
		polishedSilver.ambient = FAColor::Color(0.23125f, 0.23125f, 0.23125f, 1.0f);
		polishedSilver.diffuse = FAColor::Color(0.2775f, 0.2775f, 0.2775f, 1.0f);
		polishedSilver.specular = FAColor::Color(0.773911f, 0.773911f, 0.773911f, 1.0f);
		polishedSilver.shininess = 89.6f;
		materials.emplace_back(polishedSilver);

		//GOLD
		Material gold;
		gold.ambient = FAColor::Color(0.24725f, 0.1995f, 0.0745f, 1.0f);
		gold.diffuse = FAColor::Color(0.75164f, 0.60648f, 0.22648f, 1.0f);
		gold.specular = FAColor::Color(0.628281f, 0.555802f, 0.366065f, 1.0f);
		gold.shininess = 51.2f;
		materials.emplace_back(gold);

		//POLISHED GOLD material
		Material polishedGold;
		polishedGold.ambient = FAColor::Color(0.24725f, 0.2245f, 0.0645f, 1.0f);
		polishedGold.diffuse = FAColor::Color(0.34615f, 0.3143f, 0.0903f, 1.0f);
		polishedGold.specular = FAColor::Color(0.797357f, 0.723991f, 0.208006f, 1.0f);
		polishedGold.shininess = 83.2f;
		materials.emplace_back(polishedGold);

	}

	void BuildLightSources()
	{
		Light pointLightSource;
		pointLightSource.ambient = FAColor::Color(0.2f, 0.2f, 0.2f, 1.0f);
		pointLightSource.diffuse = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		pointLightSource.specular = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		pointLightSource.position = FAMath::Vector3D(0.0f, 0.0f, -50.0f);
		pointLightSource.lightSourceType = POINT_LIGHT;
		pointLightSource.direction = FAMath::Vector3D(0.0f, 0.0f, 0.0f);
		lightSources[0] = pointLightSource;

		Light directionalLightSource;
		directionalLightSource.ambient = FAColor::Color(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLightSource.diffuse = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLightSource.specular = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLightSource.position = FAMath::Vector3D(0.0f, 0.0f, 0.0f);
		directionalLightSource.lightSourceType = DIRECTIONAL_LIGHT;
		directionalLightSource.direction = FAMath::Vector3D(0.0f, -1.0f, 0.0f);
		lightSources[1] = directionalLightSource;

		//unused light sources so set their types to -1 so that in the shader they get ignored.
		lightSources[2].lightSourceType = -1;
		lightSources[3].lightSourceType = -1;

	}

	void BuildShapes()
	{
		//Define properties of each shape.
		shapes.emplace_back(std::make_unique<FAShapes::Box>(1.0f, 1.0f, 1.0f));

		shapes.emplace_back(std::make_unique<FAShapes::Pyramid>(1.0f, 1.0f, 1.0f));

		shapes.emplace_back(std::make_unique<FAShapes::Cylinder>(1.0f, 1.0f, FAColor::Color(), true));

		shapes.emplace_back(std::make_unique<FAShapes::Cone>(1.0f, 1.0f, FAColor::Color(), true));

		shapes.emplace_back(std::make_unique<FAShapes::Sphere>(1.0f, FAColor::Color()));
	}

	void BuildCamera()
	{
		camera.SetCameraPosition(vec3(0.0f, 0.0f, -12.5f));
		camera.SetAspectRatio((float)renderingWindow.GetWidth() / renderingWindow.GetHeight());
	}

	void BuildScene()
	{
		shadingScene = std::make_unique<FARender::RenderScene>(
			renderingWindow.GetWidth(), renderingWindow.GetHeight(),
			renderingWindow.GetWindowHandle(), true, true);
	}

	void BuildShaders()
	{
		shadingScene->CompileShader(SHADING_VS, L"Shaders/Shading_VS.hlsl", "vsMain", "vs_5_1");

		shadingScene->CompileShader(SHADING_PS, L"Shaders/Shading_PS.hlsl", "psMain", "ps_5_1");

		shadingScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		shadingScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		shadingScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		shadingScene->CreateRootDescriptor(0, 0); //object cb
		shadingScene->CreateRootDescriptor(0, 1); //pass cb
		shadingScene->CreateRootDescriptor(0, 2); //material cb
		shadingScene->CreateRootDescriptor(0, 3); //light cb
		shadingScene->CreateRootSignature(0, 0); //root signature
	}

	void BuildVertexAndIndexList()
	{
		int k = 0;
		for (auto& i : shapes)
		{
			size_t numTriangles{ i->GetNumTriangles() };

			//Describe the draw arguments for each shape.
			i->SetDrawArguments(numTriangles * 3, indexList.size(), vertexList.size(), k);

			//store the vertices of each shape.
			vertexList.insert(vertexList.end(), i->GetLocalVertices(),
				i->GetLocalVertices() + i->GetNumVertices());

			//store the indices of each shape.
			for (int j = 0; j < numTriangles; ++j)
			{
				indexList.push_back(i->GetTriangle(j).GetP0Index());
				indexList.push_back(i->GetTriangle(j).GetP1Index());
				indexList.push_back(i->GetTriangle(j).GetP2Index());
			}
			++k;
		}
	}

	void BuildVertexAndIndexBuffers()
	{
		shadingScene->CreateStaticBuffer(SHAPES_VERTEX_BUFFER, vertexList.data(),
			vertexList.size() * sizeof(FAShapes::Vertex), sizeof(FAShapes::Vertex));

		shadingScene->CreateStaticBuffer(SHAPES_INDEX_BUFFER, indexList.data(),
			indexList.size() * sizeof(unsigned int), DXGI_FORMAT_R32_UINT);

		//execute commands
		shadingScene->ExecuteAndFlush();
		shadingScene->ReleaseUploaders();
	}

	void BuildConstantBuffers()
	{
		shadingScene->CreateDynamicBuffer(OBJECTCB, shapes.size() * sizeof(ObjectConstants), nullptr, sizeof(ObjectConstants));
		shadingScene->CreateDynamicBuffer(PASSCB, sizeof(PassConstants), nullptr, sizeof(PassConstants));
		shadingScene->CreateDynamicBuffer(MATERIALCB, materials.size() * sizeof(Material), nullptr, sizeof(Material));
		shadingScene->CreateDynamicBuffer(LIGHTCB, lightSources.size() * sizeof(Light), nullptr, sizeof(Light));

	}

	void BuildPSOs()
	{
		shadingScene->CreatePSO(SHADING_PSO, D3D12_FILL_MODE_SOLID, TRUE,
			SHADING_VS, SHADING_PS, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 4);
	}

	void BuildText()
	{
		//Frames per second text
		framesPerSecond = FARender::Text(FAMath::Vector4D(), L"", 15.0f, FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f));

		WindowProc::ResizeText(renderingWindow.GetWidth(), renderingWindow.GetHeight());
	}
}