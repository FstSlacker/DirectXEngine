#include "./Includes/ParticleSystemData.hlsli"
#include "./Includes/TransformData.hlsli"

cbuffer cParticleSystemParams : register(b1)
{
    ParticleSystemParams PSParams;
};

ConsumeStructuredBuffer<Particle> ParticlesInjected : register(u0);
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
    int index = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * THREAD_IN_GROUP_TOTAL * PSParams.ParticlesCount_DeltaTime_GroupDim.z + groupIndex;

    [flatten]
    if (index >= PSParams.ParticlesCount_DeltaTime_GroupDim.x)
        return;

    Particle particle = ParticlesInjected.Consume();
    
    if(particle.LifeTime > 0)
    {
        ParticlesDest.Append(particle);
    }

}