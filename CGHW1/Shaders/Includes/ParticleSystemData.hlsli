struct ParticleSystemParams
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