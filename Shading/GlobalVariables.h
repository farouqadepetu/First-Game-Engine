#pragma once

#define MAX_NUM_LIGHTS 4

#include "FAWindow.h"
#include "FATime.h"
#include "FARenderScene.h"
#include "FAThreeDimensional.h"
#include "FACamera.h"
#include "FAText.h"

//Constant data related to objects.
struct ObjectConstants
{
	FAMath::Matrix4x4 localToWorld;						//bytes 0-63
	FAMath::Matrix4x4 inverseTransposeLocalToWorld;		//bytes 64-127

	FAMath::Matrix4x4 pad0;								//bytes 128-191
	FAMath::Matrix4x4 pad1;								//bytes 192-255
};

//Constant data not related to objects.
struct PassConstants
{
	FAMath::Matrix4x4 view;				//bytes 0-63
	FAMath::Matrix4x4 projection;		//bytes 64-127
	FAMath::Vector3D cameraPosition;	//bytes 128-139
	unsigned int shadingType;			//bytes 140-143

	FAMath::Matrix4x4 pad2;				//bytes 144-207
	FAMath::Vector4D pad3;				//bytes 208-223
	FAMath::Vector4D pad4;				//bytes 224-239
	FAMath::Vector4D pad5;				//bytes 240-255
};

//Material properties
struct Material
{
	FAColor::Color ambient;		//bytes 0-15
	FAColor::Color diffuse;		//bytes 16-31
	FAColor::Color specular;	//bytes 32-47
	float shininess;			//bytes 48-51

	FAMath::Matrix4x4 pad0;		//bytes 52-115
	FAMath::Matrix4x4 pad1;		//bytes 116-179
	FAMath::Matrix4x4 pad2;		//bytes 180-243
	FAMath::Vector3D pad3;		//bytes 244-255
};

//Light source properties
struct Light
{
	FAColor::Color ambient;		//bytes 0-15
	FAColor::Color diffuse;		//bytes 16-31
	FAColor::Color specular;	//bytes 32-47

	//for point lights
	FAMath::Vector3D position;	//bytes 48-59

	int lightSourceType;		//bytes 60-63

	//for direction lights
	FAMath::Vector3D direction;	//bytes 64-75

	float pad5;					//bytes 76-79
};

namespace GlobalVariables
{
	//All variables that need to be accessed by the window procedure.
	inline bool isAppPaused{ false }; //true if the application is paused, false otherwise.
	inline bool isMinimized{ false }; //true of the window is minimized.
	inline bool isMaximized{ false }; //true of the window is maximized.
	inline bool playAnimation{ false }; //true if we want to rotate the object.

	//Used in the window procedure to store the new width and height of the window when user resizes it.
	inline FAWindow::Window* window{ nullptr };

	//Used in the window procedure to stop the time if the application is paused and starts it if the application is no longer paused.
	//Also used to get the time between frames.
	inline FATime::Time frameTime;

	//Used in the window procedure to call the resize function when the user resizes the window.
	inline FARender::RenderScene* scene{ nullptr };

	//Enums to help keep track of the keys of mapped values.
	enum ShaderNames { GOURAUD_VS, GOURAUD_PS, PHONG_VS, PHONG_PS, BLINN_PHONG_VS, BLINN_PHONG_PS, SHADING_VS, SHADING_PS };
	enum InputElementDescriptionNames { VS_INPUT_LAYOUT };
	enum BufferNames { VERTEX_BUFFER, INDEX_BUFFER, OBJECTCB, PASSCB, MATERIALCB, LIGHTCB };
	enum PSONames { SHADING_PSO };
	enum TextNames { FRAMES_PER_SECOND };
	enum LightSourceNames { POINT_LIGHT, DIRECTIONAL_LIGHT, POINT_PLUS_DIRECTIONAL_LIGHT };
	enum SelectionNames { SHADING, SHAPES, MATERIALS, LIGHT_SOURCE };

	//vector to store pointers to 3D shapes.
	inline std::vector<std::unique_ptr<FAShapes::ThreeDimensionalShapeAbstract>> shapes;

	//Vector to store all the vertices of the 3D shapes.
	inline std::vector<FAShapes::Vertex> vertexList;

	//Vector to store all the indices of the 3D shapes.
	inline std::vector<unsigned int> indexList;

	//Camera for the scene
	inline FACamera::Camera camera;

	//Vector store all the materials.
	inline std::vector<Material> materials;

	//Vector to store all the light sources.
	inline std::vector<Light> lightSources(MAX_NUM_LIGHTS);

	inline FARender::Text framesPerSecond;

	inline FARender::Text selectionArrow;

	inline std::vector<unsigned int> currentSelection;
	inline unsigned int currentArrow{ 0 };

	inline unsigned int numSelections{ 0 };
	inline unsigned int numShading{ 0 };
	inline unsigned int numShapes{ 0 };
	inline unsigned int numMaterials{ 0 };
	inline unsigned int numLightSources{ 0 };

	inline std::vector<std::vector<FARender::Text>> selections;

	inline float textSize{ 15.0f };
	inline FAColor::Color textColor(1.0f, 1.0f, 1.0f);
	inline float heightIncrease{ 20.0f };
}