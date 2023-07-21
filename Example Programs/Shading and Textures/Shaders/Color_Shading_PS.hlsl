#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"
#include "ShadingFunctions.hlsl"

float4 psMain(vertexOutput vout) : SV_TARGET
{
	float3 pColor = float3(0.0f, 0.0f, 0.0f);

	Material material = mat;
	material.diffuse = objectConstantData.color * mat.diffuse;
    material.ambient = objectConstantData.color * mat.ambient;

	vout.worldNormal = normalize(vout.worldNormal);

	for (int i = 0; i < MAX_NUM_LIGHTS; ++i)
	{
		pColor += ComputePointLightBlinnPhong(lights[i], material, vout.worldNormal, vout.worldPosition,
			passConstantData.cameraPosition);
	}

	return float4(pColor, 1.0f);
}