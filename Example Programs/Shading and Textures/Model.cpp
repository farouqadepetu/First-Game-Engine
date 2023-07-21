#include "Model.h"

namespace MVC
{
	Model::Model()
	{}

	FARender::RenderScene* Model::GetScene()
	{
		return mScene.get();
	}

	FACamera::Camera& Model::GetCamera()
	{
		return mCamera;
	}

	FAProjection::PerspectiveProjection& Model::GetPerspectiveProjection()
	{
		return mPerspectiveProjection;
	}

	FAShapes::ThreeDimensionalShape* Model::GetShape(unsigned int index)
	{
		return mShapes.at(index);
	}

	FAShapes::Sphere& Model::GetPointLight(unsigned int index)
	{
		return mPointLights.at(index);
	}

	FATime::Time& Model::GetFrameTime()
	{
		return mFrameTime;
	}

	const Material& Model::GetMaterial() const
	{
		return mMaterial;
	}

	const std::vector<Light>& Model::GetLightSources() const
	{
		return mLightSources;
	}

	std::vector<Light>& Model::GetLightSources()
	{
		return mLightSources;
	}

	void Model::ResetCamera()
	{
		mCamera.LookAt(FAMath::Vector4D(-4.0f, 2.0f, -6.0f, 1.0f), FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f),
			FAMath::Vector4D(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Model::ResetShape(unsigned int currentShape)
	{
		mShapes.at(currentShape)->SetOrientation(FAMath::Quaternion());
	}

	void Model::ResetPointLights()
	{
		mPointLights.at(0).GetShape().SetPosition(FAMath::Vector4D(0.0f, 0.0f, -2.5f, 1.0f));
		mPointLights.at(1).GetShape().SetPosition(FAMath::Vector4D(0.0f, 0.0f, 2.5f, 1.0f));
		mPointLights.at(2).GetShape().SetPosition(FAMath::Vector4D(0.0f, -2.5f, 0.0f, 1.0f));
		mPointLights.at(3).GetShape().SetPosition(FAMath::Vector4D(0.0f, 2.5f, 0.0f, 1.0f));

		for (auto& i : mPointLights)
		{
			i.GetShape().SetOrientation(FAMath::Quaternion());
		}
	}

	void Model::BuildShapes()
	{
		//Define properties of each shape.
		box.InitializeBox(1.0f, 1.0f, 1.0f, FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(), FAColor::Color(0.0f, 1.0f, 0.0f, 1.0f));
		pyramid.InitializePyramid(1.0f, 1.0f, 1.0f, FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(), FAColor::Color(1.0f, 1.0f, 0.0f, 1.0f));
		sphere.InitializeSphere(1.0f, FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(), FAColor::Color(1.0f, 0.0f, 0.0f, 1.0f));
		cylinder.InitializeCylinder(1.0f, 1.0f, FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(), FAColor::Color(0.0f, 0.0f, 1.0f, 1.0f));
		cone.InitializeCone(1.0f, 1.0f, FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f), FAMath::Quaternion(), FAColor::Color(0.0f, 1.0f, 1.0f, 1.0f));

		mShapes.push_back(&sphere.GetShape());
		mShapes.push_back(&cylinder.GetShape());
		mShapes.push_back(&cone.GetShape());
		mShapes.push_back(&box.GetShape());
		mShapes.push_back(&pyramid.GetShape());
	}

	void Model::BuildPointLights()
	{
		for(unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			mPointLights.emplace_back(FAShapes::Sphere());
			mPointLights.at(i).InitializeSphere(0.1f, mLightSources.at(i).position, FAMath::Quaternion(), FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	void Model::BuildMaterial()
	{
		mMaterial.ambient = FAColor::Color(0.2f, 0.2f, 0.2f, 1.0f);
		mMaterial.diffuse = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mMaterial.specular = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mMaterial.shininess = 500.0f;
	}

	void Model::BuildLightSources()
	{
		mLightSources.resize(MAX_NUM_LIGHTS);

		mLightSources.at(0).color = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mLightSources.at(0).position = FAMath::Vector4D(0.0f, 0.0f, -2.5f, 1.0f);
		mLightSources.at(0).direction = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
		mLightSources.at(0).lightSourceType = POINT_LIGHT;

		mLightSources.at(1).color = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mLightSources.at(1).position = FAMath::Vector4D(0.0f, 0.0f, 2.5f, 1.0f);
		mLightSources.at(1).direction = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
		mLightSources.at(1).lightSourceType = POINT_LIGHT;

		mLightSources.at(2).color = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mLightSources.at(2).position = FAMath::Vector4D(0.0f, -2.5f, 0.0f, 1.0f);
		mLightSources.at(2).direction = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
		mLightSources.at(2).lightSourceType = POINT_LIGHT;

		mLightSources.at(3).color = FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mLightSources.at(3).position = FAMath::Vector4D(0.0f, 2.5f, 0.0f, 1.0f);
		mLightSources.at(3).direction = FAMath::Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
		mLightSources.at(3).lightSourceType = POINT_LIGHT;
	}

	void Model::BuildCamera()
	{
		mCamera.SetProperties(FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f),
			FAMath::Vector4D(1.0f, 0.0f, 0.0f, 0.0f), FAMath::Vector4D(0.0f, 1.0f, 0.0f, 0.0f), FAMath::Vector4D(0.0f, 0.0f, 1.0f, 0.0f),
			5.0f, 0.25f);

		mCamera.LookAt(FAMath::Vector4D(-4.0f, 2.0f, -6.0f, 1.0f), FAMath::Vector4D(0.0f, 0.0f, 0.0f, 1.0f),
			FAMath::Vector4D(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void Model::BuildPerspectiveProjection(unsigned int width, unsigned int height)
	{
		mPerspectiveProjection.SetProperties(1.0f, 100.0f, 45.0f, (float)width / height);
	}

	void Model::BuildScene(unsigned int width, unsigned int height, HWND windowHandle)
	{
		mScene = std::make_unique<FARender::RenderScene>(width, height, windowHandle);
	}

	void Model::BuildShaders()
	{
		mScene->CompileShader(EARTH_SHADING_VS, L"Shaders/Earth_Shading_VS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(EARTH_SHADING_PS, L"Shaders/Earth_Shading_PS.hlsl", "psMain", "ps_5_1");

		mScene->CompileShader(MOON_SUN_SHADING_VS, L"Shaders/Moon_Sun_Shading_VS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(MOON_SUN_SHADING_PS, L"Shaders/Moon_Sun_Shading_PS.hlsl", "psMain", "ps_5_1");

		mScene->CompileShader(EARTH_MOON_SUN_NO_SHADING_VS, L"Shaders/Earth_Moon_Sun_No_Shading_VS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(EARTH_MOON_SUN_NO_SHADING_PS, L"Shaders/Earth_Moon_Sun_No_Shading_PS.hlsl", "psMain", "ps_5_1");

		mScene->CompileShader(COLOR_SHADING_VS, L"Shaders/Color_Shading_VS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(COLOR_SHADING_PS, L"Shaders/Color_Shading_PS.hlsl", "psMain", "ps_5_1");

		mScene->CompileShader(COLOR_NO_SHADING_VS, L"Shaders/Color_No_Shading_VS.hlsl", "vsMain", "vs_5_1");
		mScene->CompileShader(COLOR_NO_SHADING_PS, L"Shaders/Color_No_Shading_PS.hlsl", "psMain", "ps_5_1");

		mScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
		mScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16);
		mScene->CreateInputElementDescription(VS_INPUT_LAYOUT, "TEXCOORDS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32);

		mScene->CreateRootDescriptor(EARTH_ROOT_PARAMETER, OBJECT_REGISTER); //object cb register b0
		mScene->CreateRootDescriptor(EARTH_ROOT_PARAMETER, PASS_REGISTER); //pass cb register b1
		mScene->CreateRootDescriptor(EARTH_ROOT_PARAMETER, MATERIAL_REGISTER); //material cb register b2
		mScene->CreateRootDescriptor(EARTH_ROOT_PARAMETER, LIGHT_REGISTER); //light cb register b3

		mScene->CreateRootDescriptor(MOON_SUN_ROOT_PARAMETER, OBJECT_REGISTER); //object cb register b0
		mScene->CreateRootDescriptor(MOON_SUN_ROOT_PARAMETER, PASS_REGISTER); //pass cb register b1
		mScene->CreateRootDescriptor(MOON_SUN_ROOT_PARAMETER, MATERIAL_REGISTER); //material cb register b2
		mScene->CreateRootDescriptor(MOON_SUN_ROOT_PARAMETER, LIGHT_REGISTER); //light cb register b3

		mScene->CreateRootDescriptor(EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, OBJECT_REGISTER); //object cb register b0
		mScene->CreateRootDescriptor(EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, PASS_REGISTER); //pass cb register b1
		mScene->CreateRootDescriptor(EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, MATERIAL_REGISTER); //material cb register b2
		mScene->CreateRootDescriptor(EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, LIGHT_REGISTER); //light cb register b3

		mScene->CreateRootDescriptor(COLOR_ROOT_PARAMETER, OBJECT_REGISTER); //object cb register b0
		mScene->CreateRootDescriptor(COLOR_ROOT_PARAMETER, PASS_REGISTER); //pass cb register b1
		mScene->CreateRootDescriptor(COLOR_ROOT_PARAMETER, MATERIAL_REGISTER); //material cb register b2
		mScene->CreateRootDescriptor(COLOR_ROOT_PARAMETER, LIGHT_REGISTER); //light cb register b3
	}

	void Model::BuildVertexAndIndexBuffers()
	{
		std::vector<FAShapes::Vertex> vertices;
		std::vector<FAShapes::Triangle> triangles;

		FAShapes::CreateBox(vertices, triangles);
		box.GetShape().SetDrawArguments(triangles.size() * 3, mIndexList.size(), mVertexList.size(), 0, L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
		vertices.clear();
		triangles.clear();

		FAShapes::CreatePyramid(vertices, triangles);
		pyramid.GetShape().SetDrawArguments(triangles.size() * 3, mIndexList.size(), mVertexList.size(), 1, L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
		vertices.clear();
		triangles.clear();

		FAShapes::CreateCylinder(vertices, triangles);
		cylinder.GetShape().SetDrawArguments(triangles.size() * 3, mIndexList.size(), mVertexList.size(), 2, L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
		vertices.clear();
		triangles.clear();

		FAShapes::CreateCone(vertices, triangles);
		cone.GetShape().SetDrawArguments(triangles.size() * 3, mIndexList.size(), mVertexList.size(), 3, L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}
		vertices.clear();
		triangles.clear();

		FAShapes::CreateSphere(vertices, triangles);
		sphere.GetShape().SetDrawArguments(triangles.size() * 3, mIndexList.size(), mVertexList.size(), 4, L"OBJECTCB", 0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		unsigned int constantDataIndex{ 5 };
		for (auto& i : mPointLights)
		{
			//Describe the draw arguments for each point light.
			i.GetShape().SetDrawArguments(triangles.size() * 3, mIndexList.size(), mVertexList.size(), constantDataIndex, L"OBJECTCB",
				0, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			++constantDataIndex;
		}

		mVertexList.insert(mVertexList.end(), vertices.begin(), vertices.end());

		for (const auto& i : triangles)
		{
			mIndexList.push_back(i.GetP0Index());
			mIndexList.push_back(i.GetP1Index());
			mIndexList.push_back(i.GetP2Index());
		}


		mScene->CreateStaticBuffer(SHAPES_VERTEX_BUFFER, mVertexList.data(),
			mVertexList.size() * sizeof(FAShapes::Vertex), sizeof(FAShapes::Vertex));

		mScene->CreateStaticBuffer(SHAPES_INDEX_BUFFER, mIndexList.data(),
			mIndexList.size() * sizeof(unsigned int), DXGI_FORMAT_R32_UINT);
	}

	void Model::BuildConstantBuffers()
	{
		mScene->CreateDynamicBuffer(L"OBJECTCB", (mShapes.size() + mPointLights.size()) * sizeof(ObjectConstantBuffer), 
			nullptr, sizeof(ObjectConstantBuffer));

		mScene->CreateDynamicBuffer(PASSCB, sizeof(PassConstantBuffer), nullptr, sizeof(PassConstantBuffer));
		mScene->CreateDynamicBuffer(MATERIALCB, sizeof(MaterialConstantBuffer), &mMaterial, sizeof(PassConstantBuffer));
		mScene->CreateDynamicBuffer(LIGHTCB, sizeof(LightConstantBuffer), nullptr, sizeof(LightConstantBuffer));
	}

	void Model::BuildTextures()
	{
		mScene->CreateTextureViewHeap(4);
		mScene->CreateStaticBuffer(MOON_TEX, L"Textures/moon.dds", FARender::TEX2D, 0);
		mScene->CreateStaticBuffer(SUN_TEX, L"Textures/sun.dds", FARender::TEX2D, 1);
		mScene->CreateStaticBuffer(EARTH_TEX, L"Textures/earth.dds", FARender::TEX2D, 2);
		mScene->CreateStaticBuffer(EARTH_SPECULAR_TEX, L"Textures/earthSpecular.dds", FARender::TEX2D, 3);

		mScene->CreateDescriptorRange(EARTH_DESC_RANGE, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, 0); //texture cb register t0-t1
		mScene->CreateDescriptorTable(EARTH_ROOT_PARAMETER, EARTH_DESC_RANGE);

		mScene->CreateDescriptorRange(MOON_SUN_DESC_RANGE, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, 0); //texture cb register t0-t1
		mScene->CreateDescriptorTable(MOON_SUN_ROOT_PARAMETER, MOON_SUN_DESC_RANGE);

		//texture cb register t0-t2
		mScene->CreateDescriptorRange(EARTH_MOON_SUN_NO_SHADING_DESC_RANGE, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, 0); 
		mScene->CreateDescriptorTable(EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, EARTH_MOON_SUN_NO_SHADING_DESC_RANGE);

		//static sampler register s0
		mScene->CreateStaticSampler(0, D3D12_FILTER_ANISOTROPIC,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0);
	}

	void Model::BuildRootSignature()
	{
		mScene->CreateRootSignature(EARTH_SHADING_ROOT_SIG, EARTH_ROOT_PARAMETER, 0);
		mScene->CreateRootSignature(MOON_SUN_SHADING_ROOT_SIG, MOON_SUN_ROOT_PARAMETER, 0);
		mScene->CreateRootSignature(EARTH_MOON_SUN_NO_SHADING_ROOT_SIG, EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, 0);
		mScene->CreateRootSignature(COLOR_SHADING_ROOT_SIG, COLOR_ROOT_PARAMETER);
	}

	void Model::BuildPSOs()
	{
		mScene->CreatePSO(EARTH_SHADING_PSO, D3D12_FILL_MODE_SOLID, FALSE, EARTH_SHADING_VS, EARTH_SHADING_PS,
			VS_INPUT_LAYOUT, EARTH_SHADING_ROOT_SIG, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		mScene->CreatePSO(MOON_SUN_SHADING_PSO, D3D12_FILL_MODE_SOLID, FALSE, MOON_SUN_SHADING_VS, MOON_SUN_SHADING_PS,
			VS_INPUT_LAYOUT, MOON_SUN_SHADING_ROOT_SIG, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		mScene->CreatePSO(EARTH_MOON_SUN_NO_SHADING_PSO, D3D12_FILL_MODE_SOLID, FALSE, EARTH_MOON_SUN_NO_SHADING_VS, 
			EARTH_MOON_SUN_NO_SHADING_PS,
			VS_INPUT_LAYOUT, EARTH_MOON_SUN_NO_SHADING_ROOT_SIG, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		mScene->CreatePSO(COLOR_SHADING_PSO, D3D12_FILL_MODE_SOLID, FALSE, COLOR_SHADING_VS, COLOR_SHADING_PS,
			VS_INPUT_LAYOUT, COLOR_SHADING_ROOT_SIG, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		mScene->CreatePSO(COLOR_NO_SHADING_PSO, D3D12_FILL_MODE_SOLID, FALSE, COLOR_NO_SHADING_VS, COLOR_NO_SHADING_PS,
			VS_INPUT_LAYOUT, COLOR_SHADING_ROOT_SIG, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

		mScene->CreatePSO(WIREFRAME_PSO, D3D12_FILL_MODE_WIREFRAME, FALSE, COLOR_NO_SHADING_VS, COLOR_NO_SHADING_PS,
			VS_INPUT_LAYOUT, COLOR_SHADING_ROOT_SIG, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}

	void Model::InitializeScene(const FAWindow::Window& window)
	{
		BuildMaterial();
		BuildLightSources();
		BuildShapes();
		BuildPointLights();
		BuildCamera();
		BuildPerspectiveProjection(window.GetWidth(), window.GetHeight());
		BuildScene(window.GetWidth(), window.GetHeight(), window.GetWindowHandle());
		BuildShaders();
		BuildVertexAndIndexBuffers();
		BuildConstantBuffers();
		BuildTextures();
		BuildRootSignature();
		BuildPSOs();

		mScene->ExecuteAndFlush();
	}
}