#pragma once

#include "Window.h"
#include "GameTime.h"
#include "RenderScene.h"
#include "Camera.h"
#include "PerspectiveProjection.h"
#include "Sphere.h"
#include "Box.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Pyramid.h"
#include "CreateShapes.h"
#include "Text.h"

namespace GlobalVariables
{
	//All variables that need to be accessed by the window procedure.
	inline bool isAppPaused{ false }; //true if the application is paused, false otherwise.
	inline bool isMinimized{ false }; //true of the window is minimized.
	inline bool isMaximized{ false }; //true of the window is maximized.
	inline bool isSolid{ true };	//true if in solid mode.
	inline bool isMSAAEnabled{ false }; //true if MSAA is enabled.
	inline bool isTextEnabled{ true }; //true if text is enabled.

	//Used in the window procedure to store the new width and height of the window when user resizes it.
	inline RenderingEngine::Window* window{ nullptr };

	//Used in the window procedure to stop the time if the application is paused and starts it if the application is no longer paused.
	//Also used to get the time between frames.
	inline RenderingEngine::Time frameTime;

	//Used in the window procedure to call the resize function when the user resizes the window.
	inline RenderingEngine::RenderScene* scene{ nullptr };

	struct ObjectConstants
	{
		mat4 MVP;
		RenderingEngine::Color color;

		mat4 pad0;
		mat4 pad1;
		mat3 pad2;
		vec3 pad3;
	};;

	//Constant data not related to objects.
	struct PassConstants
	{
		mat4 view;
		mat4 projection;
		mat4 pad1;
		mat4 pad2;
	};

	//To store the view and projection matrices of a scenes camera.
	inline PassConstants constantData;

	//Enums to help keep track of the keys of mapped values.
	enum ShaderNames { VERTEX_SHADER, PIXEL_SHADER };
	enum InputElementDescriptionNames { VS_INPUT_LAYOUT };
	enum BufferNames { VERTEX_BUFFER, INDEX_BUFFER, OBJECTCB, PASSCB };
	enum ObjectNames { SHAPES };
	enum PSONames { SOLID, SOLID_MSAA, WIRE, WIRE_MSAA };
	enum TextNames { FRAMES_PER_SECOND, INSTRUCTIONS };

	inline ShapesEngine::Box box;
	inline ShapesEngine::Pyramid pyramid;
	inline ShapesEngine::Cone cone;
	inline ShapesEngine::Cylinder cylinder;
	inline ShapesEngine::Sphere sphere;

	//vector to store pointers to 3D shapes.
	inline std::vector<ShapesEngine::ThreeDimensionalShape*> shapes;

	//Vector to store all the vertices of the 3D shapes.
	inline std::vector<ShapesEngine::Vertex> vertexList;

	inline std::vector<ShapesEngine::Triangle> triangleList;

	//Vector to store all the indices of the 3D shapes.
	inline std::vector<unsigned int> indexList;

	//Camera for the scene
	inline RenderingEngine::Camera camera;

	inline RenderingEngine::PerspectiveProjection pProjection;

	//Vector to store all of the text to render.
	inline std::vector<RenderingEngine::Text> textList;

	inline RenderingEngine::Window fpsText;
	inline HFONT textFont;

	inline vec2 lastMousePosition;
}