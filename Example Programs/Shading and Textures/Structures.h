#pragma once

#include "MathEngine.h"
#include "Color.h"

#define MAX_NUM_LIGHTS 4

struct ObjectConstants
{
	MathEngine::Matrix4x4 localToWorld;
	MathEngine::Matrix4x4 inverseTransposeLocalToWorld;
	RenderingEngine::Color color;
};

struct PassConstants
{
	MathEngine::Matrix4x4 view;				//bytes 0-63
	MathEngine::Matrix4x4 projection;		//bytes 64-127
	MathEngine::Vector3D cameraPosition;	//bytes 128-139
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
	RenderingEngine::Color color;		//bytes 0-15

	//for point lights
	MathEngine::Vector3D position;		//bytes 16-27

	int pad0{ 0 };						//bytes 28-31

	//for direction lights
	MathEngine::Vector3D direction;		//bytes 32-43

	int lightSourceType{ -1 };			//bytes 44-47
};


//Constant data related to objects.
struct ObjectConstantBuffer
{
	ObjectConstants objectConstants;						//bytes 0-143

	MathEngine::Matrix4x4 pad0;								//bytes 144-207
	MathEngine::Matrix3x3 pad1;								//bytes 208-243
	MathEngine::Vector3D pad2;								//bytes 244-255
};

//Constant data not related to objects.
struct PassConstantBuffer
{
	PassConstants passConstants;			//bytes 0-139

	MathEngine::Matrix4x4 pad2;				//bytes 140-203
	MathEngine::Matrix3x3 pad3;				//bytes 204-239
	MathEngine::Vector4D pad4;				//bytes 240-255
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

struct LightConstantBuffer
{
	Light lightSources[MAX_NUM_LIGHTS]; //bytes 0-191
	MathEngine::Matrix4x4 pad0;			//bytes 192-255
};
