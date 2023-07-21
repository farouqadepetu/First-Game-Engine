#include "Structures.hlsl"

#define MAX_NUM_LIGHTS 4
#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1

//object constant buffer
cbuffer ObjectCB : register(b0)
{
	ObjectConstants objectConstantData;
};

//pass constant buffer
cbuffer PassCB : register(b1)
{
	PassConstants passConstantData;
};

//Material properties
cbuffer MaterialCB : register(b2)
{
	Material mat;
};

//Light properties
cbuffer LightCB : register (b3)
{
	Light lights[MAX_NUM_LIGHTS];
};