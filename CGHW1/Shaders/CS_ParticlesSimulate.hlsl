cbuffer cTransformParams : register(b0)
{
	float4x4 WorldViewProj;
	float4x4 World;
    float4 ViewPos;
    float4x4 View;
    float4x4 Proj;
};

cbuffer cParticleSystemParams : register(b1)
{
    float4 ParticlesCount_DeltaTime_GroupDim;
};

struct Particle // описание структуры на GPU
{
    float3 Position;
    float3 Velocity;
    float Size;
};

ConsumeStructuredBuffer<Particle> ParticlesSrc : register(u0);
AppendStructuredBuffer<Particle> ParticlesDest : register(u1);

#define BLOCK_SIZE 256
#define THREAD_IN_GROUP_TOTAL 256

[numthreads(BLOCK_SIZE, 1, 1)]
void CSMain(
    uint3 groupID : SV_GroupID,
    uint3 groupThreadID : SV_GroupThreadID,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    uint groupIndex : SV_GroupIndex
)
{
    int index = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * THREAD_IN_GROUP_TOTAL * ParticlesCount_DeltaTime_GroupDim.z + groupIndex;

    [flatten]
    if (index >= ParticlesCount_DeltaTime_GroupDim.x)
        return;

    Particle particle = ParticlesSrc.Consume();

    float3 position = particle.Position;
    float3 velocity = particle.Velocity;

    particle.Position = position + velocity * ParticlesCount_DeltaTime_GroupDim.y;
    particle.Velocity = velocity;

    ParticlesDest.Append(particle);
}