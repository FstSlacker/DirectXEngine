cbuffer cCameraTransform
{
    float4x4 WorldViewProj;
};

struct VS_IN
{
    float4 position : POSITIONT;
};

struct PS_IN
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
    output.position = mul(float4(input.position.xyz, 1.0f), WorldViewProj);
    
    return output;
}