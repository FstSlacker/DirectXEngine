#include "./Includes/ParticleSystemData.hlsli"
#include "./Includes/TransformData.hlsli"

cbuffer cTransform : register(b0)
{
    TransformData Transforms;
}

cbuffer cParticleSystemParams : register(b1)
{
    ParticleSystemParams ParticleSystem;
};

Texture2D WorldPosTex : register(t0);
Texture2D NormalsTex : register(t1);

RWStructuredBuffer<Particle> Particles : register(u0);
RWStructuredBuffer<float2> SortedParticles : register(u1);
AppendStructuredBuffer<uint> DeadParticles : register(u2);

#define BLOCK_SIZE 256
#define THREAD_IN_GROUP_TOTAL 256


float3 GetViewSpaceWorldPos(float2 screenPos)
{
    float2 pixel = float2(screenPos.x * ParticleSystem.Size.z, screenPos.y * ParticleSystem.Size.w);
    
    float4 pos = WorldPosTex.Load(float3(pixel, 0.0f));
    
    float4 viewPos = mul(float4(pos.xyz, 1.0f), Transforms.View);
    
    return viewPos.xyz;
}

float3 GetNormalFromTex(float2 screenPos)
{
    float2 pixel = float2(screenPos.x * ParticleSystem.Size.z, screenPos.y * ParticleSystem.Size.w);
    
    float4 n = NormalsTex.Load(float3(pixel, 0.0f));
    
    return n.xyz;
}


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

    uint pId = (uint)SortedParticles[index].x;
    
    Particle particle = Particles[pId];
    
    particle.LifeTime -= ParticleSystem.ParticlesCount_DeltaTime_GroupDim.y;
    
    if(particle.LifeTime > 0)
    {   
        
        float3 viewSpaceParticlePos = mul(float4(particle.Position, 1.0f), Transforms.View).xyz;
        
        float4 screenSpaceParticlePos = mul(float4(viewSpaceParticlePos, 1.0f), Transforms.Proj);
        screenSpaceParticlePos.x = (screenSpaceParticlePos.x / screenSpaceParticlePos.w) * 0.5f + 0.5f;
        screenSpaceParticlePos.y = -(screenSpaceParticlePos.y / screenSpaceParticlePos.w) * 0.5f + 0.5f;
        
        if (saturate(screenSpaceParticlePos.x) == screenSpaceParticlePos.x && saturate(screenSpaceParticlePos.y) == screenSpaceParticlePos.y)
        {
            float3 viewSpaceDepthPos = GetViewSpaceWorldPos(screenSpaceParticlePos.xy);
            
            if ((viewSpaceParticlePos.z > viewSpaceDepthPos.z) && (viewSpaceParticlePos.z < viewSpaceDepthPos.z + 1.0f))
            {
                float3 normal = GetNormalFromTex(screenSpaceParticlePos.xy);
                
                float3 newVelocity = reflect(particle.Velocity, normal) * 0.8f;
                
                particle.Velocity = newVelocity;
            }

        }
        
        particle.Velocity += ParticleSystem.Gravity.xyz * ParticleSystem.ParticlesCount_DeltaTime_GroupDim.y;
        particle.Position += particle.Velocity * ParticleSystem.ParticlesCount_DeltaTime_GroupDim.y;
        
        particle.Color = lerp(ParticleSystem.EndColor, ParticleSystem.StartColor, (float4) (particle.LifeTime / particle.MaxLifeTime));
        particle.Size = lerp(ParticleSystem.Size.y, ParticleSystem.Size.x, (particle.LifeTime / particle.MaxLifeTime));

        
        float3 lengthV = Transforms.ViewPos.xyz - particle.Position.xyz;
        SortedParticles[index].y = -length(lengthV);
        
        Particles[pId] = particle;
    }
    else
    {
        SortedParticles[index] = float2(pId, 1000.0f);
        DeadParticles.Append(pId);
        
        Particles[pId].Size = 10.0f;
        Particles[pId].Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    
}