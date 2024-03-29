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
    float4x4 viewMatrix; //bytes 0-63
    float4x4 projectionMatrix; //bytes 64-127
    float3 cameraPosition; //bytes 128-139
    uint shadingType; //bytes 140-143
};

//Material data
cbuffer Material : register(b2)
{
	Material mat;
};

//Light data
cbuffer LightCB : register (b3)
{
	Light lightSources[MAX_NUM_LIGHTS];
};

float4 psMain(vertexOutput vout) : SV_TARGET
{
	float4 pColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (shadingType == GOURAUD)
	{
		pColor = vout.Color;
	}
	else if (shadingType == PHONG || shadingType == BLINN_PHONG) //phong or blinn phong shading
	{
		float3 totalColor = float3(0.0f, 0.0f, 0.0f);

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

		pColor = float4(totalColor, mat.mAmbient.w);
	}
	else if (shadingType == FLAT_PHONG || shadingType == FLAT_BLINN)
	{
		float3 totalColor = float3(0.0f, 0.0f, 0.0f);

		//take the parital derivatives of the world position of the pixel
		//with respect to screen-space coordiantes x and y to get two
		//vectors on the plane the point is a part of and take the cross product to get the normal of that plane
		float3 x = ddx(vout.worldPosition.xyz);
		float3 y = ddy(vout.worldPosition.xyz);
		float3 n = cross(x, y);
		n = normalize(n);
		float4 pixelNormal = float4(n, 0.0f);

		//sums all of the contributions from each light source
		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			if (lightSources[i].lightSourceType == POINT_LIGHT)
			{
				totalColor += ComputePointLight(lightSources[i], mat, pixelNormal.xyz,
					vout.worldPosition.xyz, cameraPosition, shadingType);
			}
			else if (lightSources[i].lightSourceType == DIRECTIONAL_LIGHT)
			{
				totalColor += ComputeDirectionalLight(lightSources[i], mat, pixelNormal.xyz,
					vout.worldPosition.xyz, cameraPosition, shadingType);
			}
		}

		pColor = float4(totalColor, 1.0f);
	}

	return pColor;
}