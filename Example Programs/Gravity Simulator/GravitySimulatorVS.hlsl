struct InputVertex
{
    float4 inputPosition : POSITION;
    float4 inputNormal : NORMAL;
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
    vout.outputPosition = mul(vin.inputPosition, MVP);
    vout.outputNormal = vin.inputNormal;
    vout.outputTexCoords = vin.inputTexCoords;
    
    return vout;
}