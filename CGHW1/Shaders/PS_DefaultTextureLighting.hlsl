cbuffer cLightBuf : register(b0)
{
    float3 ambientColor;
    float ambientIntensity;
    
    float3 diffuseColor;
    float diffuseIntensity;
    
    float3 cameraPosition;
    float specularIntensity;
    float specularPower;
    
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
    
    float3 ambient = ambientColor * ambientIntensity;
    
    float3 dirToLight = lightPosition - input.worldPos;
    
    float distToLight = length(dirToLight);
    
    if(distToLight == 0 || lightRange == 0)
        return float4(ambient * texColor, 1.0f);
    
    dirToLight /= distToLight;
    
    float diffuseDist = max(1.0f - distToLight / lightRange, 0.0f);
    
    float totalDiffuseIntensity = diffuseDist * diffuseIntensity;
    
    float3 diffuse = max(dot(dirToLight, input.normal), 0) * totalDiffuseIntensity * diffuseColor;
    
    float3 w = input.normal * dot(dirToLight, input.normal);
    float3 r = w * 2.0f - dirToLight;
    
    float3 specular = totalDiffuseIntensity * diffuseColor * specularIntensity * pow(max(0.0f, dot(normalize(r), normalize(cameraPosition - input.worldPos))), specularPower);
    
    float3 resColor = (ambient + diffuse) * texColor.rgb + specular;
    
    return float4(resColor, 1.0f);
}