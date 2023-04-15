#include "ShadingFunctions.hlsl"

//output struct
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
	float4 Color : COLOR;
	float4 worldPosition: POSITION;
	float4 worldNormal : NORMAL;
};

//pass constant buffer
cbuffer PassConstantBuffer : register(b1)
{
	float4x4 viewMatrix;		//bytes 0-63
	float4x4 projectionMatrix;  //bytes 64-127
	float3 cameraPosition;		//bytes 128-139
	uint shadingType;			//bytes 140-143

	float4x4 pad2;				//bytes 144-207
	float4 pad3;				//bytes 208-223
	float4 pad4;				//bytes 224-239
	float4 pad5;				//bytes 240-255
};

//Material data
cbuffer Material : register(b2)
{
	float4 mAmbient;	//bytes 0-15
	float4 mDiffuse;	//bytes 16-31
	float4 mSpecular;	//bytes 32-47
	float mShininess;	//bytes 48-51

	float4x4 padding0;	//bytes 52-115
	float4x4 padding1;	//bytes 116-179
	float4x4 padding2;	//bytes 180-243
	float3 padding3;	//bytes 244-255
};

//Light data
cbuffer LightCB : register (b3)
{
	Light lightSources[MAX_NUM_LIGHTS];
	float4 padding4;
};

float4 psMain(vertexOutput vout) : SV_TARGET
{
	float4 pColor;
	if (shadingType == GOURAUD)
	{
		pColor = vout.Color;
	}
	else if (shadingType == PHONG || shadingType == BLINN_PHONG) //phong or blinn phong shading
	{
		float3 totalColor = float3(0.0f, 0.0f, 0.0f);

		Material mat = { mAmbient, mDiffuse, mSpecular, mShininess };

		vout.worldNormal = normalize(vout.worldNormal);

		//sums all of the contributions from each light source
		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			if (lightSources[i].lightSourceType == POINT_LIGHT)
			{
				totalColor += ComputePointLight(lightSources[i], mat, vout.worldNormal.xyz,
					vout.worldPosition.xyz, cameraPosition, shadingType);
			}
			else if (lightSources[i].lightSourceType == DIRECTIONAL_LIGHT)
			{
				totalColor += ComputeDirectionalLight(lightSources[i], mat, vout.worldNormal.xyz,
					vout.worldPosition.xyz, cameraPosition, shadingType);
			}
		}

		pColor = float4(totalColor, mAmbient.w);
	}
	else if (shadingType == FLAT_PHONG || shadingType == FLAT_BLINN)
	{

		float3 totalColor = float3(0.0f, 0.0f, 0.0f);

		Material mat = { mAmbient, mDiffuse, mSpecular, mShininess };

		//take the parital derivatives of the world position of the pixel
		//with respect to screen-space coordiantes x and y to get two
		//vectors on the plane the point is a part of and take the cross product to get the normal of that plane
		float3 x = ddx(vout.worldPosition.xyz);
		float3 y = ddy(vout.worldPosition.xyz);
		float3 n = cross(x, y);
		n = normalize(n);

		//sums all of the contributions from each light source
		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			if (lightSources[i].lightSourceType == POINT_LIGHT)
			{
				totalColor += ComputePointLight(lightSources[i], mat, n,
					vout.worldPosition.xyz, cameraPosition, shadingType);
			}
			else if (lightSources[i].lightSourceType == DIRECTIONAL_LIGHT)
			{
				totalColor += ComputeDirectionalLight(lightSources[i], mat, n,
					vout.worldPosition.xyz, cameraPosition, shadingType);
			}
		}

		pColor = float4(totalColor, mAmbient.w);
	}

	return pColor;
}