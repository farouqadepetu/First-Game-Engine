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
	float3 cameraPosition;		//bytes 128-139
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
	float3 position;		//bytes 16-27
	
    int pad0;				//bytes 28-31

	//for direction lights
	float3 direction;		//bytes 32-43

	int type;				//bytes 44-47
};


#endif