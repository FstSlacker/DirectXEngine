struct GBufferData
{
    float4 Diffuse;
    float4 Emissive;
    float3 Normal;
    float4 Specular;
    float3 WorldPos;
};

struct TransformData
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4 ViewPos;
};

struct LightData
{
    float4 Position;
    float4 Direction;
    float4 Params; // x - range, y - constAtt, z - linearAtt, w - quadricAtt
    float4 Color; // a - intensity
};

Texture2D DiffuseSpecMap : register(t0);
Texture2D EmissiveMap : register(t1);
Texture2D NormalsMap : register(t2);
Texture2D SpecularMap : register(t3);
Texture2D WorldPosMap : register(t4);

SamplerState Sampler : register(s0);

cbuffer cTransformData : register(b0)
{
    TransformData Transforms;
};

cbuffer cLightData : register(b1)
{
    LightData Light;
};

struct LightingResult
{
    float3 Diffuse;
    float3 Specular;
};

GBufferData ReadGBuffer(float2 screenPos)
{
    GBufferData data = (GBufferData) 0;
    
    data.Diffuse = DiffuseSpecMap.Load(float3(screenPos.x, screenPos.y, 0));
    data.Emissive = EmissiveMap.Load(float3(screenPos.x, screenPos.y, 0));
    data.Normal = NormalsMap.Load(float3(screenPos.x, screenPos.y, 0)).xyz;
    data.Specular = SpecularMap.Load(float3(screenPos.x, screenPos.y, 0));
    data.WorldPos = WorldPosMap.Load(float3(screenPos.x, screenPos.y, 0)).xyz;

    return data;
}

float3 CalculateDiffuse(float3 dirToLight, float3 normal)
{
    float res = max(0, dot(dirToLight, normal));
    
    return Light.Color.xyz * res * Light.Color.a;
}

float3 CalculateSpecular(float3 dirToCamera, float3 dirToLight, float3 normal, float specPow)
{
    float3 r = normalize(reflect(-dirToLight, normal));
    float rDotV = max(0, dot(r, dirToCamera));

    return Light.Color.xyz * pow(rDotV, specPow) * Light.Color.a;
}

float CalculateAttenuation(float dist)
{
    float att = (Light.Params.y + Light.Params.z * dist + Light.Params.w * dist * dist);
    return max(1.0f - att / Light.Params.x, 0.0f);
}

float CalculateSpotCone(float3 dirToLight)
{
    float minCos = cos(Light.Direction.w);
    float maxCos = (minCos + 1.0f) / 2.0f;

    float cosAngle = dot(Light.Direction.xyz, -dirToLight);

    return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(float3 dirToCamera, float3 vertPos, float3 norm, float specPow)
{
    LightingResult res;

    float3 dirToLight = (Light.Position.xyz - vertPos).xyz;

    float distToLight = length(dirToLight);

    if (distToLight == 0.0f || Light.Params.x == 0.0f)
    {
        res.Diffuse = (float4) 0;
        res.Specular = (float4) 0;
        return res;
    }
        
    
    dirToLight = dirToLight / distToLight;

    float att = CalculateAttenuation(distToLight);
    float spotIntensity = CalculateSpotCone(dirToLight);

    res.Diffuse = CalculateDiffuse(dirToLight, norm) * att * spotIntensity;
    res.Specular = CalculateSpecular(dirToCamera, dirToLight, norm, specPow) * att * spotIntensity;

    return res;
}

struct PS_IN
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

struct PS_OUT
{
    float4 color : SV_Target0;
};

float4 PSMain(PS_IN input) : SV_Target
{
    float4 color;
    GBufferData gBuf = ReadGBuffer(input.position.xy);

    float3 dirToCamera = normalize(Transforms.ViewPos.xyz - gBuf.WorldPos.xyz);
    
    LightingResult lightRes = DoSpotLight(dirToCamera, gBuf.WorldPos, gBuf.Normal, gBuf.Specular.a);
    
    float4 diffuse = float4(gBuf.Diffuse.xyz * lightRes.Diffuse, 1.0f);
    float4 specular = float4(gBuf.Specular.xyz * lightRes.Specular, 1.0f);
    
    color = diffuse + specular;
    
    return color;
}