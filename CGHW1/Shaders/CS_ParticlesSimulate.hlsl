#include "./Includes/ParticleSystemData.hlsli"

cbuffer cParticleSystemParams : register(b1)
{
    ParticleSystemParams ParticleSystem;
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
    int index = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * THREAD_IN_GROUP_TOTAL * ParticleSystem.ParticlesCount_DeltaTime_GroupDim.z + groupIndex;

    [flatten]
    if (index >= ParticleSystem.ParticlesCount_DeltaTime_GroupDim.x)
        return;

    Particle particle = ParticlesSrc.Consume();
    
    particle.LifeTime -= ParticleSystem.ParticlesCount_DeltaTime_GroupDim.y;
    
    if(particle.LifeTime > 0)
    {
        particle.Velocity += ParticleSystem.Gravity.xyz * ParticleSystem.ParticlesCount_DeltaTime_GroupDim.y;
        particle.Position += particle.Velocity * ParticleSystem.ParticlesCount_DeltaTime_GroupDim.y;
        
        particle.Color = lerp(ParticleSystem.EndColor, ParticleSystem.StartColor, (float4) (particle.LifeTime / particle.MaxLifeTime));
        particle.Size = lerp(ParticleSystem.Size.y, ParticleSystem.Size.x, (particle.LifeTime / particle.MaxLifeTime));

        ParticlesDest.Append(particle);
    }
    
}