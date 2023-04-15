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

	float4x4 pad2;				//bytes 140-203
	float4 pad3;				//bytes 204-219
	float4 pad4;				//bytes 220-235
	float4 pad5;				//bytes 236-251
	float pad6;					//bytes 252-255
};

//main function
vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float4 localPos = float4(vin.inputPosition, 1.0f);

	//Transform the position to world space.
	vout.worldPosition = mul(float4(vin.inputPosition, 1.0f), localToWorldMatrix);

	//Transform the normal to world space.
	vout.worldNormal = mul(float4(vin.normal, 0.0f), inverseTransposeLocalToWorldMatrix);

	//MVP matrix
	float4x4 MVP = mul(localToWorldMatrix, mul(viewMatrix, projectionMatrix));

	//Transform to the position to homogenous clip space
	vout.outputPosition = mul(localPos, MVP);

	return vout;
}