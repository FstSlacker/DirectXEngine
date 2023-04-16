cbuffer cBuf : register(b0)
{
	float4x4 WorldViewProj;
	float4x4 World;
    float4 ViewPos;
    float4x4 View;
    float4x4 Proj;
};

struct Particle // описание структуры на GPU
{
    float3 Position;
    float3 Velocity;
    float Size;
};

StructuredBuffer<Particle> Particles : register(t0); // буфер частиц


struct PS_IN // описывает вертекс на выходе из Vertex Shader
{
    float4 Position : SV_POSITION;
};

struct PS_OUT // цвет результирующего пикселя
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
    data.Position = mul(data.Position, Proj);

    return data;
}

[maxvertexcount(4)] // результат работы GS – 4 вертекса, которые образуют TriangleStrip
void GSMain(point VS_OUT input[1], inout TriangleStream<PS_IN> stream)
{
    PS_IN pointOut = (PS_IN) 0;
    
    Particle p = Particles[input[0].vertexID];

    float4 worldPosition = float4(p.Position, 1);
    
    pointOut.Position = mul(worldPosition, View);
    
    const float size = p.Size; // размер конченого квадрата
    
	// описание квадрата
    stream.Append(GetOffsetPoint(pointOut, float2(-1, -1) * size));
    stream.Append(GetOffsetPoint(pointOut, float2(-1, 1) * size));
    stream.Append(GetOffsetPoint(pointOut, float2(1, -1) * size));
    stream.Append(GetOffsetPoint(pointOut, float2(1, 1) * size));

	// создать TriangleStrip
    stream.RestartStrip();
}

float4 PSMain(PS_IN input) : SV_Target0
{
    float4 output;

    output = float4((float3) 1, 1);
	
    return output;
}