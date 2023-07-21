#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"
#include "ShadingFunctions.hlsl"

Texture2D diffuseMap : register(t0, space0);
SamplerState anisotropicSampler : register(s0);

float4 psMain(vertexOutput vout) : SV_TARGET
{
	return diffuseMap.Sample(anisotropicSampler, vout.texCoords);
}