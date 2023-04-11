#pragma once

#include "FAWindow.h"
#include "FATime.h"
#include "FARenderScene.h"
#include "FAThreeDimensional.h"
#include "FACamera.h"
#include "FAText.h"

namespace GlobalVariables
{
	//All variables that need to be accessed by the window procedure.
	inline bool isAppPaused{ false }; //true if the application is paused, false otherwise.
	inline bool isMinimized{ false }; //true of the window is minimized.
	inline bool isMaximized{ false }; //true of the window is maximized.
	inline bool isResizing{ false }; //true if the user is currently resizing the window.
	inline bool isSolid{ true };	//true if in solid mode.
	inline bool isMSAAEnabled{ false }; //true if MSAA is enabled.
	inline bool isTextEnabled{ true }; //true if text is enabled.

	//Used in the window procedure to store the new width and height of the window when user resizes it.
	inline FAWindow::Window* window{ nullptr };

	//Used in the window procedure to stop the time if the application is paused and starts it if the application is no longer paused.
	//Also used to get the time between frames.
	inline FATime::Time frameTime;

	//Used in the window procedure to call the resize function when the user resizes the window.
	inline FARender::RenderScene* scene{ nullptr };

	//Constant data related to objects.
	struct ObjectConstants
	{
		FAMath::Matrix4x4 localToWorld;
		FAMath::Matrix4x4 pad0;
		FAMath::Matrix4x4 pad1;
		FAMath::Matrix4x4 pad2;
	};

	//Constant data not related to objects.
	struct PassConstants
	{
		FAMath::Matrix4x4 view;
		FAMath::Matrix4x4 projection;
		FAMath::Matrix4x4 pad1;
		FAMath::Matrix4x4 pad2;
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

	//vector to store pointers to 3D shapes.
	inline std::vector<std::unique_ptr<FAShapes::ThreeDimensionalShapeAbstract>> shapes;

	//Vector to store all the vertices of the 3D shapes.
	inline std::vector<FAShapes::Vertex> vertexList;

	//Vector to store all the indices of the 3D shapes.
	inline std::vector<unsigned int> indexList;

	//Camera for the scene
	inline FACamera::Camera camera;

	//Vector to store all of the text to render.
	inline std::vector<FARender::Text> textList;
}