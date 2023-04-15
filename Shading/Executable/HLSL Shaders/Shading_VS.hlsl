#include "ShadingFunctions.hlsl"

//input struct
struct vertexInput
{
	float3 inputPosition: POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
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

	float4x4 pad0;										//bytes 128-191
	float4x4 pad1;										//bytes 192-255
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

vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float3 totalColor = float3(0.0f, 0.0f, 0.0f);
	float4 localPos = float4(vin.inputPosition, 1.0f);

	//Transform the position and normal to world space.
	float3 worldPos = mul(localPos, localToWorldMatrix).xyz;
	float3 worldNormal = mul(float4(vin.normal, 0.0f), inverseTransposeLocalToWorldMatrix).xyz;

	if (shadingType == GOURAUD)
	{
		Material mat = { mAmbient, mDiffuse, mSpecular, mShininess };

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

		vout.Color = float4(totalColor, mAmbient.w);
	}
	else //phong or blinn phong
	{
		//let the world pos and world normal get interpolated.
		vout.worldPosition = float4(worldPos, 1.0f);
		vout.worldNormal = float4(worldNormal, 0.0f);
	}

	//MVP matrix
	float4x4 MVP = mul(localToWorldMatrix, mul(viewMatrix, projectionMatrix));

	//Transform to homogenous clip space
	vout.outputPosition = mul(localPos, MVP);

	return vout;
}

