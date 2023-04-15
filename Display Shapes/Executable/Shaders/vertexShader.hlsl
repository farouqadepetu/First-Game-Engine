//input struct
struct vertexInput
{
	float3 inputPosition: POSITION;
	float4 color: COLOR;
};

//output struct
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
	float4 Color : COLOR;
};

//object constant buffer
cbuffer ObjectConstantBuffer : register(b0)
{
	float4x4 localToWorldMatrix;
	float4x4 pad0;
	float4x4 pad1;
	float4x4 pad2;
};

//pass constant buffer
cbuffer PassConstantBuffer : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 pad3;
	float4x4 pad4;
};

//main function
vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float4 iPostion = float4(vin.inputPosition, 1.0f);

	float4x4 MVP = mul(localToWorldMatrix, mul(viewMatrix, projectionMatrix));
	
	//Transform to homogenous clip space
	vout.outputPosition = mul(iPostion, MVP);
	
	vout.Color = vin.color;

	return vout;
}