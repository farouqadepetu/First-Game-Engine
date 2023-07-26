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
		box.InitializeBox(1.0f, 1.0f, 1.0f, vec3{ 0.0f, 0.0f, 0.0f }, MathEngine::Quaternion{},RenderingEngine::Color(1.0f, 0.0f, 0.0f, 1.0f));
		pyramid.InitializePyramid(1.0f, 1.0f, 1.0f, vec3{ 2.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(1.0f, 1.0f, 0.0f, 1.0f));
		sphere.InitializeSphere(1.0f, vec3{ 4.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(0.0f, 1.0f, 0.0f, 1.0f));
		cylinder.InitializeCylinder(1.0f, 1.0f, vec3{ 6.5f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(0.0f, 0.0f, 1.0f, 1.0f));
		cone.InitializeCone(1.0f, 1.0f, vec3{ 9.0f, 0.0f, 0.0f }, MathEngine::Quaternion{}, RenderingEngine::Color(0.0f, 1.0f, 1.0f, 1.0f));

		shapes.push_back(&box.GetShape());
		shapes.push_back(&pyramid.GetShape());
		shapes.push_back(&sphere.GetShape());
		shapes.push_back(&cylinder.GetShape());
		shapes.push_back(&cone.GetShape());

	}

	void BuildCamera()
	{
		SetProperties(camera, vec3{ 5.0f, 0.0f, -15.0f }, vec3{ 1.0f, 0.0f, 0.0f }, vec3{ 0.0f, 1.0f, 0.0f }, vec3{ 0.0f, 0.0f, 1.0f },
			5.0f, 0.25f);
	}

	void BuildPerspectiveProjection(unsigned int width, unsigned int height)
	{
		SetProperties(pProjection, 1.0f, 100.0f, 45.0f, (float)width / height);
	}

	void BuildShaders(RenderingEngine::RenderScene& scene)
	{
		scene.CompileShader(VERTEX_SHADER, L"Shaders/vertexShader.hlsl", "vsMain", "vs_5_1");

		scene.CompileShader(PIXEL_SHADER, L"Shaders/pixelShader.hlsl", "psMain", "ps_5_1");

		scene.CreateInputElementDescription(VS_INPUT_LAYOUT, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		scene.CreateRootDescriptor(0, 0); //object cb
		scene.CreateRootDescriptor(0, 1); //pass cb
		scene.CreateRootSignature(0, 0);
	}

	void BuildVertexAndIndexList(RenderingEngine::RenderScene& scene)
	{
		std::vector<ShapesEngine::Vertex> vertices;
		std::vector<ShapesEngine::Triangle> triangles;

		ShapesEngine::CreateBox(vertices, triangles);
		ShapesEngine::SetDrawArguments(box.GetShape(), triangles.size() * 3, indexList.size(), vertexList.size(), 0, 
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
		ShapesEngine::SetDrawArguments(pyramid.GetShape(), triangles.size() * 3, indexList.size(), vertexList.size(), 1,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
		ShapesEngine::SetDrawArguments(sphere.GetShape(), triangles.size() * 3, indexList.size(), vertexList.size(), 2,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
		ShapesEngine::SetDrawArguments(cylinder.GetShape(), triangles.size() * 3, indexList.size(), vertexList.size(), 3,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
		ShapesEngine::SetDrawArguments(cone.GetShape(), triangles.size() * 3, indexList.size(), vertexList.size(), 4,
			L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		vertexList.insert(vertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			indexList.push_back(i.p0);
			indexList.push_back(i.p1);
			indexList.push_back(i.p2);
		}
	}

	void BuildVertexAndIndexBuffers(RenderingEngine::RenderScene& scene)
	{
		scene.CreateStaticBuffer(L"Vertex Buffer", vertexList.data(),
			vertexList.size() * sizeof(ShapesEngine::Vertex), sizeof(ShapesEngine::Vertex));

		scene.CreateStaticBuffer(L"Index Buffer", indexList.data(), indexList.size() * sizeof(unsigned int), DXGI_FORMAT_R32_UINT);

		//execute commands
		scene.ExecuteAndFlush();
	}

	void BuildConstantBuffers(RenderingEngine::RenderScene& scene)
	{
		scene.CreateDynamicBuffer(L"OBJECTCB", 5 * sizeof(ObjectConstants), nullptr, sizeof(ObjectConstants));
		scene.CreateDynamicBuffer(L"PASSCB", sizeof(PassConstants), nullptr, sizeof(PassConstants));
	}

	void BuildPSOs(RenderingEngine::RenderScene& scene)
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
		textList.emplace_back(RenderingEngine::Text(vec4{ 0.0f, 0.01f * height, 0.3f * width, 0.02f * height }, L"", 15.0f,
			RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f)));


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

		textList.emplace_back(RenderingEngine::Text(vec4{ 0.7f * width, 0.0f, (float)width, 0.0f },
			instructions, 15.0f, RenderingEngine::Color(1.0f, 1.0f, 1.0f, 1.0f)));


	}
}