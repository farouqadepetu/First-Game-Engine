//output struct from vertex shader
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
	float4 Color : COLOR;
};

float4 psMain(vertexOutput vout) : SV_TARGET
{
	//return the color
	return vout.Color;
	
}