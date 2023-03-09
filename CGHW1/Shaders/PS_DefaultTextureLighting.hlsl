cbuffer cLightBuf : register(b0)
{
    float3 ambientColor;
    float ambientIntensity;
    float3 diffuseColor;
    float diffuseIntensity;
    float3 lightPosition;
    float lightRange;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

Texture2D objTexture : register(t0);
SamplerState objSamplerState : register(s0);

float4 PSMain( PS_IN input ) : SV_Target
{
    float3 texColor = objTexture.Sample(objSamplerState, input.texCord);
    
    float3 appliedColor = ambientColor * ambientIntensity;
    
    float3 dirToLight = lightPosition - input.worldPos;
    
    float distToLight = length(dirToLight);
    
    if(distToLight == 0 || lightRange == 0)
        return float4(appliedColor * texColor, 1.0f);
    
    dirToLight /= distToLight;
    
    float diffuseDist = max(1.0f - distToLight / lightRange, 0.0f);
    
    float3 diffuse = max(dot(dirToLight, input.normal), 0) * diffuseDist * diffuseIntensity * diffuseColor;
    
    appliedColor += diffuse;
    
    float3 resColor = appliedColor * texColor;
    
    return float4(resColor, 1.0f);
}