#pragma once

#define MAX_NUM_LIGHTS 4

#include "Window.h"
#include "GameTime.h"
#include "RenderScene.h"
#include "Box.h"
#include "Pyramid.h"
#include "Sphere.h"
#include "Cone.h"
#include "Cylinder.h"
#include "CreateShapes.h"
#include "Camera.h"
#include "PerspectiveProjection.h"
#include "Text.h"
#include "DrawArguments.h"

struct ObjectConstants
{
	MathEngine::Matrix4x4 localToWorld;		
	MathEngine::Matrix4x4 inverseTransposeLocalToWorld;
};

struct PassConstants
{
	MathEngine::Matrix4x4 view;				//bytes 0-63
	MathEngine::Matrix4x4 projection;		//bytes 64-127
	MathEngine::Vector3D cameraPosition;	//bytes 128-139
	unsigned int shadingType{ 0 };			//bytes 140-143
};

struct Material
{
	RenderingEngine::Color ambient;		//bytes 0-15
	RenderingEngine::Color diffuse;		//bytes 16-31
	RenderingEngine::Color specular;	//bytes 32-47
	float shininess{ 1.0f };			//bytes 48-51
};

//Light source properties
struct Light
{
	RenderingEngine::Color ambient;		//bytes 0-15
	RenderingEngine::Color diffuse;		//bytes 16-31
	RenderingEngine::Color specular;	//bytes 32-47

	//for point lights
	MathEngine::Vector3D position;		//bytes 48-59

	int pad0{ 0 };						//bytes 60-63

	//for direction lights
	MathEngine::Vector3D direction;		//bytes 64-75

	int lightSourceType{ -1 };			//bytes 76-79
};


//Constant data related to objects.
struct ObjectConstantBuffer
{
	ObjectConstants objectConstants;					//bytes 0-127

	MathEngine::Matrix4x4 pad0;								//bytes 128-191
	MathEngine::Matrix4x4 pad1;								//bytes 192-255
};

//Constant data not related to objects.
struct PassConstantBuffer
{
	PassConstants passConstants;			//bytes 0-143

	MathEngine::Matrix4x4 pad2;				//bytes 144-207
	MathEngine::Matrix3x3 pad3;				//bytes 208-243
	MathEngine::Vector3D pad4;				//bytes 244-255
};


//Material properties
struct MaterialConstantBuffer
{
	Material material;				//bytes 0-51

	MathEngine::Matrix4x4 pad0;		//bytes 52-115
	MathEngine::Matrix4x4 pad1;		//bytes 116-179
	MathEngine::Matrix4x4 pad2;		//bytes 180-243
	MathEngine::Vector3D pad3;		//bytes 244-255
};

//The struct for the light constant buffer in the hlsl shadeer
struct LightConstantBuffer
{
	Light lightSources[MAX_NUM_LIGHTS];		//bytes 0-319
	MathEngine::Matrix4x4 pad0;				//bytes 320-383
	MathEngine::Matrix4x4 pad1;				//bytes 384-447
	MathEngine::Matrix4x4 pad2;				//bytes 448-511
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
	inline RenderingEngine::Window mainWindow;

	//The width and height of the client area of the main window
	inline RECT mainWindowClientRect{};

	//The window we render to
	inline RenderingEngine::Window renderingWindow;

	//The static text for each drop down list window.
	inline RenderingEngine::Window staticText[4];

	//The drop down list windows to select shading, shapes, materials and light sources.
	inline RenderingEngine::Window dropDownLists[4];

	//Used in the window procedure to stop the time if the application is paused and starts it if the application is no longer paused.
	//Also used to get the time between frames.
	inline RenderingEngine::Time frameTime;

	//Used in the window procedure to call the resize function when the user resizes the window.
	inline std::unique_ptr<RenderingEngine::RenderScene> shadingScene{ nullptr };

	//Enums to help keep track of the keys of mapped values.
	enum ShaderNames { GOURAUD_VS, GOURAUD_PS, PHONG_VS, PHONG_PS, BLINN_PHONG_VS, BLINN_PHONG_PS, SHADING_VS, SHADING_PS };
	enum InputElementDescriptionNames { VS_INPUT_LAYOUT };
	enum BufferNames { SHAPES_VERTEX_BUFFER, SHAPES_INDEX_BUFFER, OBJECTCB, PASSCB, MATERIALCB, LIGHTCB };
	enum PSONames { SHADING_PSO };
	enum TextNames { FRAMES_PER_SECOND };
	enum LightSourceNames { POINT_LIGHT, DIRECTIONAL_LIGHT, POINT_PLUS_DIRECTIONAL_LIGHT };
	enum SelectionNames { SHADING, SHAPES, MATERIALS, LIGHT_SOURCE };
	enum ButtonNames { PLAY_PAUSE, RESET_CAMERA, RESET_SHAPE };
	enum ShapeNames { BOX, PYRAMID, CYLINDER, CONE, SPHERE };

	inline ShapesEngine::Box box;
	inline ShapesEngine::Pyramid pyramid;
	inline ShapesEngine::Cone cone;
	inline ShapesEngine::Cylinder cylinder;
	inline ShapesEngine::Sphere sphere;

	//Vector to store all the vertices of the 3D shapes.
	inline std::vector<ShapesEngine::Vertex> vertexList;

	//Vector to store all the indices of the 3D shapes.
	inline std::vector<unsigned int> indexList;

	//Camera for the scene
	inline RenderingEngine::Camera camera;

	inline RenderingEngine::PerspectiveProjection pProjection;

	//Vector store all the materials.
	inline std::vector<Material> materials;

	//Array to store all the light sources.
	inline std::vector<Light> lightSources(MAX_NUM_LIGHTS);

	inline RenderingEngine::Text framesPerSecond;

	inline std::vector<unsigned int> currentSelection;

	inline HFONT textFont;

	inline RenderingEngine::Window buttons[3];

	inline vec2 lastMousePosition;
}