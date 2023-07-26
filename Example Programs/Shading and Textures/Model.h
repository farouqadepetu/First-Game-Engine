#pragma once

#include <vector>
#include <memory>
#include "RenderScene.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Box.h"
#include "Pyramid.h"
#include "Camera.h"
#include "PerspectiveProjection.h"
#include "GameTime.h"
#include "Window.h"
#include "Structures.h"
#include "CreateShapes.h"

namespace MVC
{
	enum ShaderNames { EARTH_SHADING_VS, EARTH_SHADING_PS, MOON_SUN_SHADING_VS, MOON_SUN_SHADING_PS, EARTH_MOON_SUN_NO_SHADING_VS,
		EARTH_MOON_SUN_NO_SHADING_PS, COLOR_SHADING_VS, COLOR_SHADING_PS, COLOR_NO_SHADING_VS, COLOR_NO_SHADING_PS };

	enum PSONames { EARTH_SHADING_PSO, MOON_SUN_SHADING_PSO, EARTH_MOON_SUN_NO_SHADING_PSO, COLOR_SHADING_PSO, COLOR_NO_SHADING_PSO,
	WIREFRAME_PSO };

	enum RootSignatureNames { EARTH_SHADING_ROOT_SIG, MOON_SUN_SHADING_ROOT_SIG, EARTH_MOON_SUN_NO_SHADING_ROOT_SIG,
		COLOR_SHADING_ROOT_SIG };

	enum InputEleDescNames { VS_INPUT_LAYOUT };

	enum DescRangeNames { EARTH_DESC_RANGE, MOON_SUN_DESC_RANGE, EARTH_MOON_SUN_NO_SHADING_DESC_RANGE };

	enum RootParameterNames { EARTH_ROOT_PARAMETER, MOON_SUN_ROOT_PARAMETER, EARTH_MOON_SUN_NO_SHADING_ROOT_PARAMETER, 
		COLOR_ROOT_PARAMETER };

	enum TextureNames { EARTH, MOON, SUN };

	enum CBShaderRegisterNames { OBJECT_REGISTER, PASS_REGISTER, MATERIAL_REGISTER, LIGHT_REGISTER };

	enum TextureShaderRegisterNames { TEXTURE_REGISTER };

	enum BufferNames { SHAPES_VERTEX_BUFFER, SHAPES_INDEX_BUFFER, OBJECTCB, PASSCB,
		MATERIALCB, LIGHTCB, EARTH_TEX, EARTH_SPECULAR_TEX, MOON_TEX, SUN_TEX };

	enum LightSourceNames { POINT_LIGHT, DIRECTIONAL_LIGHT, POINT_PLUS_DIRECTIONAL_LIGHT };

	class Model
	{
	public:
		Model();

		RenderingEngine::RenderScene* GetScene();

		RenderingEngine::Camera& GetCamera();

		RenderingEngine::PerspectiveProjection& GetPerspectiveProjection();

		ShapesEngine::Box box;
		ShapesEngine::Pyramid pyramid;
		ShapesEngine::Cone cone;
		ShapesEngine::Cylinder cylinder;
		ShapesEngine::Sphere sphere;

		ShapesEngine::ThreeDimensionalShape* GetShape(unsigned int index);

		ShapesEngine::Sphere& GetPointLight(unsigned int index);

		RenderingEngine::Time& GetFrameTime();

		const Material& GetMaterial() const;

		std::vector<Light>& GetLightSources();

		const std::vector<Light>& GetLightSources() const;
		
		void ResetCamera();
		void ResetShape(unsigned int currentShape);
		void ResetPointLights();

		void InitializeScene(const RenderingEngine::Window& window);

	private:
		void BuildShapes();
		void BuildPointLights();
		void BuildMaterial();
		void BuildLightSources();
		void BuildCamera();
		void BuildPerspectiveProjection(unsigned int width, unsigned int height);
		void BuildScene(unsigned int width, unsigned int height, HWND windowHandle);
		void BuildShaders();
		void BuildVertexAndIndexBuffers();
		void BuildConstantBuffers();
		void BuildTextures();
		void BuildRootSignature();
		void BuildPSOs();

	private:
		std::unique_ptr<RenderingEngine::RenderScene> mScene;

		std::vector<ShapesEngine::ThreeDimensionalShape*> mShapes;

		std::vector<ShapesEngine::Sphere> mPointLights;

		RenderingEngine::Camera mCamera;
		RenderingEngine::PerspectiveProjection mPerspectiveProjection;

		RenderingEngine::Time mFrameTime;

		Material mMaterial;

		std::vector<Light> mLightSources;

		//Vector to store all the vertices of the 3D shapes.
		std::vector<ShapesEngine::Vertex> mVertexList;

		//Vector to store all the indices of the 3D shapes.
		std::vector<unsigned int> mIndexList;
	};
}