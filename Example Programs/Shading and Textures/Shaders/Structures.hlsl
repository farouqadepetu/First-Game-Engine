#ifndef STRUCTURES_HLSL
#define STRUCTURES_HLSL

//constant data related to objects
struct ObjectConstants
{
	float4x4 localToWorldMatrix;						//bytes 0-63
	float4x4 inverseTransposeLocalToWorldMatrix;		//bytes 64-127
    float4 color;										//bytes 128-143
};

//constant data not related to objects
struct PassConstants
{
	float4x4 viewMatrix;		//bytes 0-63
	float4x4 projectionMatrix;  //bytes 64-127
	float4 cameraPosition;		//bytes 128-143
};

//Material properties
struct Material
{
	float4 ambient;		//bytes 0-15
	float4 diffuse;		//bytes 16-31
	float4 specular;	//bytes 32-47
	float shininess;	//bytes 48-51
};

//Light properties
struct Light
{
	float4 color;			//bytes 0-15

	//for point lights
	float4 position;		//bytes 16-31

	//for direction lights
	float4 direction;		//bytes 32-47

	int type;				//bytes 48-51

	float3 pad;				//bytes	52-63
};


#endif