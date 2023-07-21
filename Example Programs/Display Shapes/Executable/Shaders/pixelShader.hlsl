//output struct from vertex shader
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


float4 psMain(vertexOutput vout) : SV_TARGET
{
	//return the color
	return color;
	
}