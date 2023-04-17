#include "./Includes/ParticleSystemData.hlsli"
#include "./Includes/TransformData.hlsli"

cbuffer cBuf : register(b0)
{
    TransformData Transforms;
};

StructuredBuffer<Particle> Particles : register(t0);

struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Color : PARTICLE_COLOR;
};

struct PS_OUT
{
    float4 Color : SV_TARGET0;
};

struct VS_OUT
{
    uint vertexID : TEXCOORD;
};

VS_OUT VSMain(uint vertexId : SV_VertexID)
{
    VS_OUT output = (VS_OUT) 0;

    output.vertexID = vertexId;
    
    return output;
}

PS_IN GetOffsetPoint(PS_IN data, float2 offset)
{
    data.Position.xy += offset;
    data.Position = mul(data.Position, Transforms.Proj);

    return data;
}

[maxvertexcount(4)]
void GSMain(point VS_OUT input[1], inout TriangleStream<PS_IN> stream)
{
    PS_IN pointOut = (PS_IN) 0;
    
    Particle p = Particles[input[0].vertexID];

    float4 worldPosition = float4(p.Position, 1);
    
    pointOut.Position = mul(worldPosition, Transforms.View);
    pointOut.Color = p.Color;
    
    const float size = p.Size;
    
    stream.Append(GetOffsetPoint(pointOut, float2(-1, -1) * size));
    stream.Append(GetOffsetPoint(pointOut, float2(-1, 1) * size));
    stream.Append(GetOffsetPoint(pointOut, float2(1, -1) * size));
    stream.Append(GetOffsetPoint(pointOut, float2(1, 1) * size));

    stream.RestartStrip();
}

float4 PSMain(PS_IN input) : SV_Target0
{
    float4 output;

    output = input.Color;
	
    return output;
}