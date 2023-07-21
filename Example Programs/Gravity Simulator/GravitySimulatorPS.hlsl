struct OutputVertex
{
    float4 outputPosition : SV_POSITION;
    float4 outputNormal : NORMAL;
    float2 outputTexCoords : TEXCOORDS;
};

cbuffer ObjectConstantBuffer : register(b0)
{
    float4x4 MVP;
    float4 color;
}

float4 psMain(OutputVertex vout) : SV_TARGET
{
    return color;
}