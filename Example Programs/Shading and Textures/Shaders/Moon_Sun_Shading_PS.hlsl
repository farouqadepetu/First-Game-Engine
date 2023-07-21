#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"
#include "ShadingFunctions.hlsl"

Texture2D diffuseMap : register(t0, space0);
SamplerState anisotropicSampler : register(s0);

float4 psMain(vertexOutput vout) : SV_TARGET
{
	float3 pColor = float3(0.0f, 0.0f, 0.0f);

	float4 diffuse = diffuseMap.Sample(anisotropicSampler, vout.texCoords);

	Material material = mat;
	material.diffuse = diffuse * mat.diffuse;
	material.ambient = diffuse * mat.ambient;

	vout.worldNormal = normalize(vout.worldNormal);

	for (int i = 0; i < MAX_NUM_LIGHTS; ++i)
	{
		pColor += ComputePointLightBlinnPhong(lights[i], material, vout.worldNormal, vout.worldPosition,
			passConstantData.cameraPosition);
	}

	return float4(pColor, 1.0f);
}