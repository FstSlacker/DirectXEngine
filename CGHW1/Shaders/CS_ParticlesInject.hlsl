#include "./Includes/ParticleSystemData.hlsli"
#include "./Includes/TransformData.hlsli"

cbuffer cTransforms : register(b0)
{
    TransformData Transforms;
};

cbuffer cParticleSystemParams : register(b1)
{
    ParticleSystemParams PSParams;
};

RWStructuredBuffer<Particle> Particles : register(u0);
AppendStructuredBuffer<float2> SortedParticles : register(u1);
ConsumeStructuredBuffer<uint> DeadParticles : register(u2);
ConsumeStructuredBuffer<Particle> InjectedParticles : register(u3);

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

    Particle particle = InjectedParticles.Consume();
    
    if(particle.LifeTime > 0)
    {
        uint pId = DeadParticles.Consume();
        
        float3 lengthV = Transforms.ViewPos.xyz - particle.Position.xyz;
        float distance = -length(lengthV);
        
        SortedParticles.Append(float2(pId, distance));
        
        Particles[pId] = particle;
    }

}