#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"
#include "ShadingFunctions.hlsl"

float4 psMain(vertexOutput vout) : SV_TARGET
{
    return objectConstantData.color;
}