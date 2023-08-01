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
		CreateParentWindow(mainWindow, hInstance, WindowProc::MainWindowProc, RenderingEngine::Color(0.4f, 0.4f, 0.4f), L"Main Window Class", 
			L"Shading Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720);
	}

	void BuildRenderingWindow(HINSTANCE hInstance)
	{
		CreateChildWindow(renderingWindow, hInstance, mainWindow.windowHandle, 0, WindowProc::RenderingWindowProc,
			RenderingEngine::Color(), L"Rendering Window Class", L"Rendering Window",
			WS_CHILD | WS_VISIBLE, 0, 0, RenderingEngine::GetWidth(mainWindow) - dropDownListWidth, RenderingEngine::GetHeight(mainWindow));
	}

	void BuildDropDownLists(HINSTANCE hInstance)
	{
		//Set the font of the static text
		HDC hdc = GetDC(mainWindow.windowHandle);
		LOGFONT logFont = { 0 };
		logFont.lfHeight = -MulDiv(15, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfWeight = FW_BOLD;
		textFont = CreateFontIndirect(&logFont);

		unsigned int dropDownListY = 50;
		unsigned int staticTextY = 25;
		for (int i = 0; i < 4; ++i)
		{
			//Create drop down list
			CreateControlWindow(dropDownLists[i], hInstance, mainWindow.windowHandle, i, WC_COMBOBOX, L"",
				CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TILED,
				RenderingEngine::GetWidth(renderingWindow), dropDownListY, dropDownListWidth, dropDownListHeight);

			//Create static text for the drop down list
			CreateControlWindow(staticText[i], hInstance, mainWindow.windowHandle, i, L"STATIC", L"",
				WS_VISIBLE | WS_CHILD | SS_CENTER, RenderingEngine::GetWidth(renderingWindow), staticTextY, dropDownListWidth, dropDownListHeight);

			SendMessage(staticText[i].windowHandle, WM_SETFONT, (WPARAM)textFont, (LPARAM)TRUE);

			dropDownListY += dropDownListHeight;
			staticTextY += dropDownListHeight;

			currentSelection.push_back(0);
		}

		SetWindowText(staticText[SHADING].windowHandle, L"SHADING");
		SetWindowText(staticText[SHAPES].windowHandle, L"SHAPES");
		SetWindowText(staticText[MATERIALS].windowHandle, L"MATERIALS");
		SetWindowText(staticText[LIGHT_SOURCE].windowHandle, L"LIGHT SOURCE");


		//Add the options to the shading drop down list
		SendMessage(dropDownLists[SHADING].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Gouraud");
		SendMessage(dropDownLists[SHADING].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Phong");
		SendMessage(dropDownLists[SHADING].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Blinn-Phong");
		SendMessage(dropDownLists[SHADING].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Flat-Phong");
		SendMessage(dropDownLists[SHADING].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Flat-Blinn");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists[SHADING].windowHandle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//Add the options to the shape drop down list
		SendMessage(dropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Box");
		SendMessage(dropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Pyramid");
		SendMessage(dropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Cylinder");
		SendMessage(dropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Cone");
		SendMessage(dropDownLists[SHAPES].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Sphere");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists[SHAPES].windowHandle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//Add the options to the shape drop down list
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Bronze");
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Polished Bronze");
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Silver");
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Polished Silver");
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Gold");
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Polished Gold");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists[MATERIALS].windowHandle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//Add the options to the shape drop down list
		SendMessage(dropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Point");
		SendMessage(dropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Directional");
		SendMessage(dropDownLists[LIGHT_SOURCE].windowHandle, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Point + Directional");

		//Set the the first element in the shape drop down list to be the initial string that is displayed.
		SendMessage(dropDownLists[LIGHT_SOURCE].windowHandle, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		ReleaseDC(mainWindow.windowHandle, hdc);
	}

	void BuildButtons(HINSTANCE hInstance)
	{
		unsigned int buttonY = RenderingEngine::GetY(dropDownLists[3]) + 90;
		unsigned int buttonX = RenderingEngine::GetWidth(renderingWindow) + 20;

		CreateControlWindow(buttons[0], hInstance, mainWindow.windowHandle, 0, L"BUTTON", L"PLAY/PAUSE ROTATION",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_MULTILINE,
			buttonX, buttonY, buttonWidth, buttonHeight);

		buttonY += buttonHeight + 10;
		CreateControlWindow(buttons[1], hInstance, mainWindow.windowHandle, 1, L"BUTTON", L"RESET CAMERA",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_MULTILINE,
			buttonX, buttonY, buttonWidth, buttonHeight);

		buttonY += buttonHeight + 10;
		CreateControlWindow(buttons[2], hInstance, mainWindow.windowHandle, 2, L"BUTTON", L"RESET SHAPE",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_MULTILINE,
			buttonX, buttonY, buttonWidth, buttonHeight);
	}

	void BuildMaterials()
	{
		//BRONZE
		Material bronze;
		bronze.ambient = RenderingEngine::Color(0.2125f, 0.1275f, 0.054f, 1.0f);
		bronze.diffuse = RenderingEngine::Color(0.714f, 0.4284f, 0.18144f, 1.0f);
		bronze.specular = RenderingEngine::Color(0.393548f, 0.271906f, 0.166721f, 1.0f);
		bronze.shininess = 25.6f;
		materials.emplace_back(bronze);

		//POLISHED BRONZE
		Material polishedBronze;
		polishedBronze.ambient = RenderingEngine::Color(0.25f, 0.148f, 0.06475f, 1.0f);
		polishedBronze.diffuse = RenderingEngine::Color(0.4f, 0.2368f, 0.1036f, 1.0f);
		polishedBronze.specular = RenderingEngine::Color(0.774597f, 0.458561f, 0.200621f, 1.0f);
		polishedBronze.shininess = 76.8f;
		materials.emplace_back(polishedBronze);

		//SILVER
		Material silver;
		silver.ambient = RenderingEngine::Color(0.19225f, 0.19225f, 0.19225f, 1.0f);
		silver.diffuse = RenderingEngine::Color(0.50754f, 0.50754f, 0.50754f, 1.0f);
		silver.specular = RenderingEngine::Color(0.508273f, 0.508273f, 0.508273f, 1.0f);
		silver.shininess = 51.2f;
		materials.emplace_back(silver);

		//POLISHED SILVER
		Material polishedSilver;
		polishedSilver.ambient = RenderingEngine::Color(0.23125f, 0.23125f, 0.23125f, 1.0f);
		polishedSilver.diffuse = RenderingEngine::Color(0.2775f, 0.2775f, 0.2775f, 1.0f);
		polishedSilver.specular = RenderingEngine::Color(0.773911f, 0.773911f, 0.773911f, 1.0f);
		polishedSilver.shininess = 89.6f;
		materials.emplace_back(polishedSilver);

		//GOLD
		Material gold;
		gold.ambient = RenderingEngine::Color(0.24725f, 0.1995f, 0.0745f, 1.0f);
		gold.diffuse = RenderingEngine::Color(0.75164f, 0.60648f, 0.22648f, 1.0f);
		gold.specular = RenderingEngine::Color(0.628281f, 0.555802f, 0.366065f, 1.0f);
		gold.shininess = 51.2f;
		materials.emplace_back(gold);

		//POLISHED GOLD material
		Material polishedGold;
		polishedGold.ambient = RenderingEngine::Color(0.24725f, 0.2245f, 0.0645f, 1.0f);
		polishedGold.diffuse = RenderingEngine::Color(0.34615f, 0.3143f, 0.0903f, 1.0f);
		polishedGold.specular = RenderingEngine::Color(0.797357f, 0.723991f, 0.208006f, 1.0f);
		polishedGold.shininess = 83.2f;
		materials.emplace_back(polishedGold);

	}

	void BuildLightSources()
	{
		Light pointLightSource;
		pointLightSource.ambient = RenderingEngine::Color(0.2f, 0.2f, 0.2f, 1.0f);
		pointLightSource.diffuse = RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f);
		pointLightSource.specular = RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f);
		pointLightSource.position = vec3{ 0.0f, 0.0f, -10.0f };
		pointLightSource.lightSourceType = POINT_LIGHT;
		pointLightSource.direction = vec3{ 0.0f, 0.0f, 0.0f };
		lightSources[0] = pointLightSource;

		Light directionalLightSource;
		directionalLightSource.ambient = RenderingEngine::Color(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLightSource.diffuse = RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLightSource.specular = RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLightSource.position = vec3{ 0.0f, 0.0f, 0.0f };
		directionalLightSource.lightSourceType = DIRECTIONAL_LIGHT;
		directionalLightSource.direction = vec3{ 0.0f, -1.0f, 0.0f };
		lightSources[1] = directionalLightSource;
	}

	void BuildShapes()
	{
		//Define properties of each shape.
		box.InitializeBox(1.0f, 1.0f, 1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(1.0f, 0.0f, 0.0f, 1.0f));
		pyramid.InitializePyramid(1.0f, 1.0f, 1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(1.0f, 1.0f, 0.0f, 1.0f));
		sphere.InitializeSphere(1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(0.0f, 1.0f, 0.0f, 1.0f));
		cylinder.InitializeCylinder(1.0f, 1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(0.0f, 0.0f, 1.0f, 1.0f));
		cone.InitializeCone(1.0f, 1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion(), RenderingEngine::Color(0.0f, 1.0f, 1.0f, 1.0f));
	}

	void BuildCamera()
	{
		SetProperties(camera, vec3{ 0.0f, 0.0f, -5.0f }, vec3{ 1.0f, 0.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.0f }, vec3{ 0.0f, 0.0f, 1.0f, },
			5.0f, 0.25f);
	}

	void BuildPerspectiveProjection()
	{
		SetProperties(pProjection, 1.0f, 100.0f, 45.0f, (float)RenderingEngine::GetWidth(renderingWindow) / RenderingEngine::GetHeight(renderingWindow));
	}

	void BuildScene()
	{
		shadingScene = std::make_unique<RenderingEngine::RenderScene>(
			RenderingEngine::GetWidth(renderingWindow), RenderingEngine::GetHeight(renderingWindow),
			renderingWindow.windowHandle, true, true);
	}

	void BuildShaders()
	{
		shadingScene->CompileShader(SHADING_VS, L"Shaders/Shading_VS.hlsl", "vsMain", "vs_5_1");

		shadingScene->CompileShader(SHADING_PS, L"Shaders/Shading_PS.hlsl", "psMain", "ps_5_1");

		shadingScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		shadingScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		shadingScene->CreateRootDescriptor(0, 0); //object cb
		shadingScene->CreateRootDescriptor(0, 1); //pass cb
		shadingScene->CreateRootDescriptor(0, 2); //material cb
		shadingScene->CreateRootDescriptor(0, 3); //light cb
		shadingScene->CreateRootSignature(0, 0); //root signature
	}

	void BuildVertexAndIndexList()
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateBox(vertices, triangles);
		box.SetDrawArguments(RenderingEngine::MakeDrawArguments(triangles.size() * 3, indexList.size(), vertexList.size(), 0,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			indexList.push_back(i.p0);
			indexList.push_back(i.p1);
			indexList.push_back(i.p2);
		}
		vertices.clear();
		triangles.clear();

		ShapesEngine::CreatePyramid(vertices, triangles);
		pyramid.SetDrawArguments(RenderingEngine::MakeDrawArguments(triangles.size() * 3, indexList.size(), vertexList.size(), 1,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			indexList.push_back(i.p0);
			indexList.push_back(i.p1);
			indexList.push_back(i.p2);
		}
		vertices.clear();
		triangles.clear();

		ShapesEngine::CreateSphere(vertices, triangles);
		sphere.SetDrawArguments(RenderingEngine::MakeDrawArguments(triangles.size() * 3, indexList.size(), vertexList.size(), 2,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			indexList.push_back(i.p0);
			indexList.push_back(i.p1);
			indexList.push_back(i.p2);
		}
		vertices.clear();
		triangles.clear();

		ShapesEngine::CreateCylinder(vertices, triangles);
		cylinder.SetDrawArguments(RenderingEngine::MakeDrawArguments(triangles.size() * 3, indexList.size(), vertexList.size(), 3,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			indexList.push_back(i.p0);
			indexList.push_back(i.p1);
			indexList.push_back(i.p2);
		}
		vertices.clear();
		triangles.clear();

		ShapesEngine::CreateCone(vertices, triangles);
		cone.SetDrawArguments(RenderingEngine::MakeDrawArguments(triangles.size() * 3, indexList.size(), vertexList.size(), 4,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			indexList.push_back(i.p0);
			indexList.push_back(i.p1);
			indexList.push_back(i.p2);
		}
	}

	void BuildVertexAndIndexBuffers()
	{
		shadingScene->CreateStaticBuffer(SHAPES_VERTEX_BUFFER, vertexList.data(),
			vertexList.size() * sizeof(ShapesEngine::Vertex), sizeof(ShapesEngine::Vertex));

		shadingScene->CreateStaticBuffer(SHAPES_INDEX_BUFFER, indexList.data(),
			indexList.size() * sizeof(unsigned int), DXGI_FORMAT_R32_UINT);

		//execute commands
		shadingScene->ExecuteAndFlush();
	}

	void BuildConstantBuffers()
	{
		shadingScene->CreateDynamicBuffer(L"OBJECTCB", sizeof(ObjectConstantBuffer), nullptr, sizeof(ObjectConstantBuffer));
		shadingScene->CreateDynamicBuffer(PASSCB, sizeof(PassConstantBuffer), nullptr, sizeof(PassConstantBuffer));
		shadingScene->CreateDynamicBuffer(MATERIALCB, sizeof(MaterialConstantBuffer), nullptr, sizeof(MaterialConstantBuffer));
		shadingScene->CreateDynamicBuffer(LIGHTCB, sizeof(LightConstantBuffer), nullptr, sizeof(LightConstantBuffer));
	}

	void BuildPSOs()
	{
		shadingScene->CreatePSO(SHADING_PSO, D3D12_FILL_MODE_SOLID, TRUE,
			SHADING_VS, SHADING_PS, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 4);
	}

	void BuildText()
	{
		//Frames per second text
		framesPerSecond = RenderingEngine::Text(MathEngine::Vector4D(), L"", 15.0f, RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f));

		WindowProc::ResizeText(RenderingEngine::GetWidth(renderingWindow), RenderingEngine::GetHeight(renderingWindow));
	}
}