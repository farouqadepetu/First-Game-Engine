//input struct
struct vertexInput
{
	float4 inputPosition: POSITION;
	float4 normal : NORMAL;
	float2 texCoords : TEXCOORDS;
};

//output struct
struct vertexOutput
{
	float4 outputPosition : SV_POSITION;
	float4 worldPosition: POSITION;
	float4 worldNormal : NORMAL;
	float2 texCoords : TEXCOORDS;
};