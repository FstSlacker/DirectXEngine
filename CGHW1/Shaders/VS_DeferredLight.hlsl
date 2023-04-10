cbuffer cTransformBuf : register(b0)
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4 ViewPos;
};

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct PS_IN
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.position = mul(float4(input.pos.xyz, 1.0f), WorldViewProj);
    output.texCoord = input.texCord;
    
    return output;
}