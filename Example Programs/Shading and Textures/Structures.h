#pragma once

#include "FAMathEngine.h"
#include "FAColor.h"

#define MAX_NUM_LIGHTS 4

struct ObjectConstants
{
	FAMath::Matrix4x4 localToWorld;
	FAMath::Matrix4x4 inverseTransposeLocalToWorld;
	FAColor::Color color;
};

struct PassConstants
{
	FAMath::Matrix4x4 view;				//bytes 0-63
	FAMath::Matrix4x4 projection;		//bytes 64-127
	FAMath::Vector4D cameraPosition;	//bytes 128-143
};

struct Material
{
	FAColor::Color ambient;		//bytes 0-15
	FAColor::Color diffuse;		//bytes 16-31
	FAColor::Color specular;	//bytes 32-47
	float shininess{ 1.0f };	//bytes 48-51
};

//Light source properties
struct Light
{
	FAColor::Color color;		//bytes 0-15

	//for point lights
	FAMath::Vector4D position;	//bytes 16-31

	//for direction lights
	FAMath::Vector4D direction;	//bytes 32-47

	int lightSourceType{ -1 };	//bytes 48-51

	FAMath::Vector3D pad;		//bytes 52-63
};


//Constant data related to objects.
struct ObjectConstantBuffer
{
	ObjectConstants objectConstants;					//bytes 0-143

	FAMath::Matrix4x4 pad0;								//bytes 144-207
	FAMath::Matrix3x3 pad1;								//bytes 208-243
	FAMath::Vector3D pad2;								//bytes 244-255
};

//Constant data not related to objects.
struct PassConstantBuffer
{
	PassConstants passConstants;		//bytes 0-143

	FAMath::Matrix4x4 pad2;				//bytes 144-207
	FAMath::Vector4D pad3;				//bytes 208-223
	FAMath::Vector4D pad4;				//bytes 224-239
	FAMath::Vector4D pad5;				//bytes 240-255
};


//Material properties
struct MaterialConstantBuffer
{
	Material material;			//bytes 0-51

	FAMath::Matrix4x4 pad0;		//bytes 52-115
	FAMath::Matrix4x4 pad1;		//bytes 116-179
	FAMath::Matrix4x4 pad2;		//bytes 180-243
	FAMath::Vector3D pad3;		//bytes 244-255
};

struct LightConstantBuffer
{
	Light lightSources[MAX_NUM_LIGHTS]; //bytes 0-255
};
