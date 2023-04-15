#include "InitFunctions.h"
#include "WindowProcedure.h"
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>

using namespace GlobalVariables;

namespace Init
{
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

	void BuildCamera(unsigned int width, unsigned int height)
	{
		camera.SetCameraPosition(vec3(0.0f, 0.0f, -12.5f));
		camera.SetAspectRatio((float)width / height);
	}


	void BuildShaders(FARender::RenderScene& scene)
	{
		scene.CompileShader(SHADING_VS, L"Shaders/Shading_VS.hlsl", "vsMain", "vs_5_1");

		scene.CompileShader(SHADING_PS, L"Shaders/Shading_PS.hlsl", "psMain", "ps_5_1");

		scene.CreateInputElementDescription(VS_INPUT_LAYOUT, "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		scene.CreateInputElementDescription(VS_INPUT_LAYOUT, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		scene.CreateInputElementDescription(VS_INPUT_LAYOUT, "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);

		scene.CreateRootParameter(0, 0); //object cb
		scene.CreateRootParameter(0, 1); //pass cb
		scene.CreateRootParameter(0, 2); //material cb
		scene.CreateRootParameter(0, 3); //light cb
		scene.CreateRootSignature(0, 0); //root signature
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
		scene.CreateDynamicBuffer(2, MATERIALCB, materials.size() * sizeof(Material), nullptr, sizeof(Material));
		scene.CreateDynamicBuffer(2, LIGHTCB, lightSources.size() * sizeof(Light), nullptr, sizeof(Light));

	}

	void BuildPSOs(FARender::RenderScene& scene)
	{
		scene.CreatePSO(SHADING_PSO, D3D12_FILL_MODE_SOLID, TRUE,
			SHADING_VS, SHADING_PS, VS_INPUT_LAYOUT, 0, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, 4);
	}

	void BuildText(unsigned int width, unsigned int height)
	{
		//Frames per second text
		framesPerSecond = FARender::Text(FAMath::Vector4D(), L"", textSize, textColor);

		//Selection Arrow txt
		selectionArrow = FARender::Text(FAMath::Vector4D(), L">", textSize, textColor);

		//Shading selections
		std::vector<FARender::Text> shadingSelections;

		shadingSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Shading:", textSize, textColor));
		shadingSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Gouraud", textSize, textColor));
		shadingSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Phong", textSize, textColor));
		shadingSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Blinn-Phong", textSize, textColor));
		shadingSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Flat-Phong", textSize, textColor));
		shadingSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Flat-Blinn", textSize, textColor));
		selections.emplace_back(shadingSelections);
		currentSelection.emplace_back(1);

		//Shape selections
		std::vector<FARender::Text> shapeSelections;

		shapeSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Shape:", textSize, textColor));
		shapeSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Box", textSize, textColor));
		shapeSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Pyramid", textSize, textColor));
		shapeSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Cylinder", textSize, textColor));
		shapeSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Cone", textSize, textColor));
		shapeSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Sphere", textSize, textColor));
		selections.emplace_back(shapeSelections);
		currentSelection.emplace_back(1);

		//Material Selections
		std::vector<FARender::Text> materialSelections;

		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Material:", textSize, textColor));
		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Bronze", textSize, textColor));
		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Polished Bronze", textSize, textColor));
		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Silver", textSize, FAColor::Color(1.0f, 1.0f, 1.0f, 1.0f)));
		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Polished Silver", textSize, textColor));
		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Gold", textSize, textColor));
		materialSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Polished Gold", textSize, textColor));
		selections.emplace_back(materialSelections);
		currentSelection.emplace_back(1);

		//Light Source Selections
		std::vector<FARender::Text> lightSourceSelections;

		lightSourceSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Light Source:", textSize, textColor));
		lightSourceSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Point Light", textSize, textColor));
		lightSourceSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Directional Light", textSize, textColor));
		lightSourceSelections.emplace_back(FARender::Text(FAMath::Vector4D(), L"Point + Directional Light", textSize, textColor));
		selections.emplace_back(lightSourceSelections);
		currentSelection.emplace_back(1);

		numShading = shadingSelections.size() - 1;
		numShapes = shapeSelections.size() - 1;
		numMaterials = materialSelections.size() - 1;
		numLightSources = lightSourceSelections.size() - 1;
		numSelections = selections.size() - 1;

		WindowProc::ResizeText(width, height);
	}
}