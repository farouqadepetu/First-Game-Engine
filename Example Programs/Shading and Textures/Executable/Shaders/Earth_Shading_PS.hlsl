#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"
#include "ShadingFunctions.hlsl"

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
SamplerState anisotropicSampler : register(s0);

float4 psMain(vertexOutput vout) : SV_TARGET
{
	float3 pColor = float3(0.0f, 0.0f, 0.0f);

	float4 diffuse = diffuseMap.Sample(anisotropicSampler, vout.texCoords);
	float4 specular = specularMap.Sample(anisotropicSampler, vout.texCoords);

	Material material = mat;
	material.diffuse = diffuse * mat.diffuse;
	material.ambient = diffuse * mat.ambient;
	material.specular = specular * mat.specular;

	vout.worldNormal = normalize(vout.worldNormal);

	for (int i = 0; i < MAX_NUM_LIGHTS; ++i)
	{
		pColor += ComputePointLightBlinnPhong(lights[i], material, vout.worldNormal.xyz, vout.worldPosition.xyz,
			passConstantData.cameraPosition);
	}

	return float4(pColor, 1.0f);
}