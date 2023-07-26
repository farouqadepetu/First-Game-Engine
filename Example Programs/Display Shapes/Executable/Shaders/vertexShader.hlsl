//input struct
struct vertexInput
{
	float3 inputPosition : POSITION;
};

//output struct
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
};

//object constant buffer
cbuffer ObjectConstantBuffer : register(b0)
{
	float4x4 localToWorldMatrix;
    float4 color;
};

//pass constant buffer
cbuffer PassConstantBuffer : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

//main function
vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float4x4 MVP = mul(localToWorldMatrix, mul(viewMatrix, projectionMatrix));
	
	//Transform to homogenous clip space
    vout.outputPosition = mul(float4(vin.inputPosition, 1.0f), MVP);

	return vout;
}