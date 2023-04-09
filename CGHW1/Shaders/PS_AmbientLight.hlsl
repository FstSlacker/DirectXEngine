struct GBufferData
{
    float4 DiffuseSpec;
    float3 Emissive;
    float3 Normal;
    float3 WorldPos;
};

struct TransformData
{
    float4x4 World;
    float4x4 WorldViewProj;
};

struct LightData
{
    float4 Position;
    float4 Direction;
    float4 Params;
    float4 Color;
};

Texture2D DiffuseSpecMap : register(t0);
Texture2D EmissiveMap : register(t1);
Texture2D NormalsMap : register(t2);
Texture2D WorldPosMap : register(t3);

SamplerState Sampler : register(s0);

cbuffer cTransformData : register(b0)
{
    TransformData Transforms;
};

cbuffer cLightData : register(b1)
{
    LightData Light;
};


GBufferData ReadGBuffer(float2 screenPos)
{
    GBufferData data = (GBufferData) 0;
    
    data.DiffuseSpec = DiffuseSpecMap.Load(float3(screenPos.x, screenPos.y, 0));
    data.Emissive = EmissiveMap.Load(float3(screenPos.x, screenPos.y, 0)).xyz;
    data.Normal = NormalsMap.Load(float3(screenPos.x, screenPos.y, 0)).xyz;
    data.WorldPos = WorldPosMap.Load(float3(screenPos.x, screenPos.y, 0)).xyz;

    return data;
}

struct PS_IN
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

struct PS_OUT
{
    float4 color : SV_Target0;
};

float4 PSMain(PS_IN input) : SV_Target
{
    //PS_OUT output = (PS_OUT) 0;
    float4 color;
    GBufferData gBuf = ReadGBuffer(input.position.xy);

    color = float4(gBuf.DiffuseSpec.xyz * Light.Color.xyz * Light.Color.a, 1.0f);
    
    return color;
}