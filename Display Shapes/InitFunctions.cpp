#include "InitFunctions.h"
#include "DisplayShapesGlobalVariables.h"
#include <sstream>
#include <iomanip>

using namespace GlobalVariables;

namespace Init
{
	void BuildShapes()
	{
		//Define properties of each shape.
		shapes.emplace_back(std::make_unique<FAShapes::Box>(1.0f, 1.0f, 1.0f, FAColor::Color(1.0f, 0.0f, 0.0f, 1.0f)));
		shapes.emplace_back(std::make_unique<FAShapes::Pyramid>(1.0f, 1.0f, 1.0f, FAColor::Color(1.0f, 1.0f, 0.0f, 1.0f)));
		shapes.emplace_back(std::make_unique<FAShapes::Sphere>(1.0f, FAColor::Color(0.0f, 1.0f, 0.0f, 1.0f)));
		shapes.emplace_back(std::make_unique<FAShapes::Cylinder>(1.0f, 1.0f, FAColor::Color(0.0f, 0.0f, 1.0f, 1.0f), true));
		shapes.emplace_back(std::make_unique<FAShapes::Cone>(1.0f, 1.0f, FAColor::Color(0.0f, 1.0f, 1.0f, 1.0f), true));

		//Set the locations of each shape.
		float location{ 1.0f };
		for (auto& i : shapes)
		{
			i->SetCenter(FAMath::Vector3D(location, 0.0f, 0.0f));
			location += 2.5f;
		}
	}

	void BuildCamera(unsigned int width, unsigned int height)
	{
		camera.SetCameraPosition(vec3(5.0f, 0.0f, -12.5f));
		camera.SetAspectRatio((float)width / height);
	}


	void BuildShaders(FARender::RenderScene& scene)
	{
		scene.CompileShader(VERTEX_SHADER, L"Shaders/vertexShader.hlsl", "vsMain", "vs_5_1");

		scene.CompileShader(PIXEL_SHADER, L"Shaders/pixelShader.hlsl", "psMain", "ps_5_1");

		scene.CreateInputElementDescription(VS_INPUT_LAYOUT, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		scene.CreateInputElementDescription(VS_INPUT_LAYOUT, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		scene.CreateRootParameter(0, 0); //object cb
		scene.CreateRootParameter(0, 1); //pass cb
		scene.CreateRootSignature(0, 0);
	}

	void BuildVertexAndIndexList(FARender::RenderScene& scene)
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

	void BuildVertexAndIndexBuffers(FARender::RenderScene& scene)
	{
		scene.CreateStaticBuffer(0, VERTEX_BUFFER, vertexList.data(),
			vertexList.size() * sizeof(FAShapes::Vertex), sizeof(FAShapes::Vertex));

		scene.CreateStaticBuffer(1, INDEX_BUFFER, indexList.data(), indexList.size() * sizeof(unsigned int), 0, DXGI_FORMAT_R32_UINT);

		//execute commands
		scene.ExecuteAndFlush();
	}

	void BuildConstantBuffers(FARender::RenderScene& scene)
	{
		scene.CreateDynamicBuffer(2, OBJECTCB, shapes.size() * sizeof(ObjectConstants), nullptr, sizeof(ObjectConstants));
		scene.CreateDynamicBuffer(2, PASSCB, sizeof(PassConstants), nullptr, sizeof(PassConstants));
	}

	void BuildPSOs(FARender::RenderScene& scene)
	{
		scene.CreatePSO(SOLID, D3D12_FILL_MODE_SOLID, FALSE,
			VERTEX_SHADER, PIXEL_SHADER, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 1);

		scene.CreatePSO(SOLID_MSAA, D3D12_FILL_MODE_SOLID, TRUE,
			VERTEX_SHADER, PIXEL_SHADER, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 4);

		scene.CreatePSO(WIRE, D3D12_FILL_MODE_WIREFRAME, FALSE,
			VERTEX_SHADER, PIXEL_SHADER, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 1);

		scene.CreatePSO(WIRE_MSAA, D3D12_FILL_MODE_WIREFRAME, TRUE,
			VERTEX_SHADER, PIXEL_SHADER, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 4);
	}

	void BuildText(unsigned int width, unsigned int height)
	{
		//Frames per second text
		textList.emplace_back(FARender::Text(FAMath::Vector4D(0.0f, 0.01f * height, 0.3f * width, 0.02f * height), L"", 15.0f,
			FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f)));


		//Instructions text
		std::wstring instructions
		{
			L"Press 1 to turn on/off MSAA\n"
			L"Press 2 to turn on/off the text\n"
			L"Press 3 to switch between wireframe/solid mode\n\n"
			L"How to move the camera:\n"
			L"Use WASD or the arrow keys to move the camera\n"
			L"Click the left mouse button and move the mouse to rotate the camera" 
		};

		textList.emplace_back(FARender::Text(FAMath::Vector4D(0.7f * width, 0.0f, width, 0.0f),
			instructions, 15.0f, FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f)));


	}
}