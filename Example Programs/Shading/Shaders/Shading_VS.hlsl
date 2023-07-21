#include "ShadingFunctions.hlsl"

//input struct
struct vertexInput
{
	float4 inputPosition : POSITION;
	float4 normal : NORMAL;
};

//output struct
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
	float4 Color : COLOR;
	float4 worldPosition: POSITION;
	float4 worldNormal : NORMAL;
};

//object constant buffer
cbuffer ObjectConstantBuffer : register(b0)
{
	float4x4 localToWorldMatrix;						//bytes 0-63
	float4x4 inverseTransposeLocalToWorldMatrix;		//bytes 64-127
};

//pass constant buffer
cbuffer PassConstantBuffer : register(b1)
{
	float4x4 viewMatrix;		//bytes 0-63
	float4x4 projectionMatrix;  //bytes 64-127
	float4 cameraPosition;		//bytes 128-143
	uint shadingType;			//bytes 143-147
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

vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float3 totalColor = float3(0.0f, 0.0f, 0.0f);

	//Transform the position and normal to world space.
	float4 worldPos = mul(vin.inputPosition, localToWorldMatrix);
	float4 worldNormal = mul(vin.normal, inverseTransposeLocalToWorldMatrix);

	if (shadingType == GOURAUD)
	{
		worldNormal = normalize(worldNormal);

		//sums all of the contributions from each light source
		for (unsigned int i = 0; i < MAX_NUM_LIGHTS; ++i)
		{
			if (lightSources[i].lightSourceType == POINT_LIGHT)
			{
				totalColor += ComputePointLight(lightSources[i], mat, worldNormal, worldPos, cameraPosition, shadingType);
			}
			else if(lightSources[i].lightSourceType == DIRECTIONAL_LIGHT)
			{
				totalColor += ComputeDirectionalLight(lightSources[i], mat, worldNormal, worldPos, cameraPosition, shadingType);
			}
		}

		vout.Color = float4(totalColor, 1.0f);
	}
	else //phong or blinn phong
	{
		//let the world pos and world normal get interpolated.
		vout.worldPosition = worldPos;
		vout.worldNormal = worldNormal;
	}

	//MVP matrix
	float4x4 MVP = mul(localToWorldMatrix, mul(viewMatrix, projectionMatrix));

	//Transform to homogenous clip space
	vout.outputPosition = mul(vin.inputPosition, MVP);

	return vout;
}

