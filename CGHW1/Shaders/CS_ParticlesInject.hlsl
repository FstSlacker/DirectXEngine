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
    float4 Gravity;
    float4 Size;
    float4 StartColor;
    float4 EndColor;
};

struct Particle
{
    float3 Position;
    float3 Velocity;
    float4 Color;
    float Size;
    float LifeTime;
    float MaxLifeTime;
};

AppendStructuredBuffer<Particle> ParticlesDest : register(u1);
ConsumeStructuredBuffer<Particle> ParticlesInjected : register(u2);

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

    Particle particle = ParticlesInjected.Consume();
    
    if(particle.LifeTime > 0)
    {
        ParticlesDest.Append(particle);
    }

}