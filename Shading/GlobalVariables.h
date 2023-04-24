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
	unsigned int shadingType{ 0 };		//bytes 140-143

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
	float shininess{ 0.0f };	//bytes 48-51

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

	int lightSourceType{ 0 };		//bytes 60-63

	//for direction lights
	FAMath::Vector3D direction;	//bytes 64-75

	float pad5{ 0.0f };					//bytes 76-79
};

namespace GlobalVariables
{
	//All variables that need to be accessed by the window procedure.
	inline bool isAppPaused{ false }; //true if the application is paused, false otherwise.
	inline bool isMinimized{ false }; //true of the window is minimized.
	inline bool isMaximized{ false }; //true of the window is maximized.
	inline bool playAnimation{ false }; //true if we want to rotate the object.
	inline bool enableCameraUserInput{ true };

	inline unsigned int dropDownListWidth{ 200 };
	inline unsigned int dropDownListHeight{ 125 };

	inline unsigned int buttonWidth{ 150 };
	inline unsigned int buttonHeight{ 40 };

	//The main window we render to
	inline FAWindow::Window mainWindow;

	//The width and height of the client area of the main window
	inline RECT mainWindowClientRect{};

	//The window we render to
	inline FAWindow::Window renderingWindow;

	//The static text for each drop down list window.
	inline std::vector<FAWindow::Window> staticText;

	//The drop down list windows to select shading, shapes, materials and light sources.
	inline std::vector<FAWindow::Window> dropDownLists;

	//Used in the window procedure to stop the time if the application is paused and starts it if the application is no longer paused.
	//Also used to get the time between frames.
	inline FATime::Time frameTime;

	//Used in the window procedure to call the resize function when the user resizes the window.
	inline std::unique_ptr<FARender::RenderScene> shadingScene{ nullptr };

	//Enums to help keep track of the keys of mapped values.
	enum ShaderNames { GOURAUD_VS, GOURAUD_PS, PHONG_VS, PHONG_PS, BLINN_PHONG_VS, BLINN_PHONG_PS, SHADING_VS, SHADING_PS };
	enum InputElementDescriptionNames { VS_INPUT_LAYOUT };
	enum BufferNames { SHAPES_VERTEX_BUFFER, SHAPES_INDEX_BUFFER, OBJECTCB, PASSCB, MATERIALCB, LIGHTCB };
	enum PSONames { SHADING_PSO };
	enum TextNames { FRAMES_PER_SECOND };
	enum LightSourceNames { POINT_LIGHT, DIRECTIONAL_LIGHT, POINT_PLUS_DIRECTIONAL_LIGHT };
	enum SelectionNames { SHADING, SHAPES, MATERIALS, LIGHT_SOURCE };
	enum ButtonNames { PLAY_PAUSE, RESET_CAMERA, RESET_SHAPE };

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

	inline std::vector<unsigned int> currentSelection;

	inline HFONT textFont;

	inline std::vector<FAWindow::Window> buttons;
}