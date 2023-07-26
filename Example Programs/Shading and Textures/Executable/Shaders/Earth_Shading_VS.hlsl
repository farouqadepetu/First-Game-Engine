#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"

vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float4x4 MVP = mul(objectConstantData.localToWorldMatrix, mul(passConstantData.viewMatrix, passConstantData.projectionMatrix));

    vout.outputPosition = mul(float4(vin.inputPosition, 1.0f), MVP);
    vout.worldPosition = mul(float4(vin.inputPosition, 1.0f), objectConstantData.localToWorldMatrix);
    vout.worldNormal = mul(float4(vin.normal, 0.0f), objectConstantData.inverseTransposeLocalToWorldMatrix);
	vout.texCoords = vin.texCoords;

	return vout;
}