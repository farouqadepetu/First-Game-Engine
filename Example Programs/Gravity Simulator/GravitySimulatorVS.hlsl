struct InputVertex
{
    float3 inputPosition : POSITION;
    float3 inputNormal : NORMAL;
    float2 inputTexCoords : TEXCOORDS;
};

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

OutputVertex vsMain(InputVertex vin)
{
    OutputVertex vout;
    
    //transform to homogenous clip space
    vout.outputPosition = mul(float4(vin.inputPosition, 1.0f), MVP);
    vout.outputNormal = float4(vin.inputNormal, 0.0f);
    vout.outputTexCoords = vin.inputTexCoords;
    
    return vout;
}