#define MAX_LIGHTS 8

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D objTexture : register(t0);
Texture2D objNormalMap : register(t1);

SamplerState objSamplerState : register(s0);

struct MaterialData
{
    float4 Emissive; // 16
    float4 Ambient; // 16
    float4 Diffuse; // 16
    float4 Specular; // 16
    
    float SpecularPower; // 4
    bool UseTexture; // 4
    float _padding; // 4
    bool UseNormalMap; // 4
    //float2 Padding; // 4
};

struct LightData
{
    float4 Position; // 16 
    float4 Direction; // 16 
    float4 Color; // 16 
    
    float Intensity; // 4
    float Range; // 4

    float SpotAngle; // 4 
    float ConstantAtt; // 4
    
    float LinearAtt; // 4 
    float QuadraticAtt; // 4 
    
    int LightType; // 4 
    bool Enabled; // 4
};

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

cbuffer cMaterialBuf : register(b0)
{
    MaterialData Material;
};

cbuffer cLightBuf : register(b1)
{
    float3 AmbientColor; // 12
    float AmbientIntensity; // 4
    float3 CameraPosition; // 12
    //4
    LightData Lights[MAX_LIGHTS];
};

cbuffer cTransformBuf : register(b2)
{
    float4x4 wvpMat;
    float4x4 worldMat;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
    float4 color : COLOR;
 	float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

float4 CalculateDiffuse(LightData light, float3 dirToLight, float3 normal)
{
    float res = max(0, dot(dirToLight, normal));
    
    return light.Color * res * light.Intensity;
}

float4 CalculateSpecular(LightData light, float3 dirToCamera, float3 dirToLight, float3 normal)
{
    float3 r = normalize(reflect(-dirToLight, normal));
    float rDotV = max(0, dot(r, dirToCamera));

    return light.Color * pow(rDotV, Material.SpecularPower) * light.Intensity;
}

float CalculateAttenuation(LightData light, float dist)
{
    float att = (light.ConstantAtt + light.LinearAtt * dist + light.QuadraticAtt * dist * dist);
    return max(1.0f - att / light.Range, 0.0f);
}

LightingResult DoPointLight(LightData light, float3 dirToCamera, float4 vertPos, float3 norm)
{


    LightingResult res;

    float3 dirToLight = (light.Position - vertPos).xyz;
    float distToLight = length(dirToLight);

    dirToLight = dirToLight / distToLight;

    if (distToLight == 0.0f || light.Range == 0.0f)
    {
        res.Diffuse = (float4) 0;
        res.Specular = (float4) 0;
        return res;
    }
    
    float att = CalculateAttenuation(light, distToLight);

    res.Diffuse = CalculateDiffuse(light, dirToLight, norm) * att;
    res.Specular = CalculateSpecular(light, dirToCamera, dirToLight, norm) * att;

    return res;
}

LightingResult DoDirectionalLight(LightData light, float3 dirToCamera, float4 vertPos, float3 norm)
{
    LightingResult res;

    float3 dirToLight = -light.Direction.xyz;

    res.Diffuse = CalculateDiffuse(light, dirToLight, norm);
    res.Specular = CalculateSpecular(light, dirToCamera, dirToLight, norm);

    return res;
}

float CalculateSpotCone(LightData light, float3 dirToLight)
{
    float minCos = cos(light.SpotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f;

    float cosAngle = dot(light.Direction.xyz, -dirToLight);

    return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(LightData light, float3 dirToCamera, float4 vertPos, float3 norm)
{
    LightingResult res;

    float3 dirToLight = (light.Position - vertPos).xyz;

    float distToLight = length(dirToLight);

    if(distToLight == 0.0f || light.Range == 0.0f)
    {
        res.Diffuse = (float4)0;
        res.Specular = (float4)0;
        return res;
    }
        
    
    dirToLight = dirToLight / distToLight;

    float att = CalculateAttenuation(light, distToLight);
    float spotIntensity = CalculateSpotCone(light, dirToLight);

    res.Diffuse = CalculateDiffuse(light, dirToLight, norm) * att * spotIntensity;
    res.Specular = CalculateSpecular(light, dirToCamera, dirToLight, norm) * att * spotIntensity;

    return res;
}

LightingResult ComputeLighting(float3 vertPos, float3 normal)
{
    float3 dirToCamera = normalize(CameraPosition - vertPos.xyz);
    float4 vertPos4 = float4(vertPos, 0.0f);

    LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
    
    [unroll]
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        LightingResult res = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

        if (!Lights[i].Enabled)
            continue;

        switch (Lights[i].LightType)
        {
            case DIRECTIONAL_LIGHT:
                {
                    res = DoDirectionalLight(Lights[i], dirToCamera, vertPos4, normal);
                }
                break;

            case POINT_LIGHT:
                {
                    res = DoPointLight(Lights[i], dirToCamera, vertPos4, normal);
                }
                break;

            case SPOT_LIGHT:
                {
                    res = DoSpotLight(Lights[i], dirToCamera, vertPos4, normal);
                }
                break;
        }

        totalResult.Diffuse += res.Diffuse;
        totalResult.Specular += res.Specular;
    }

    totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);

    return totalResult;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    if (Material.UseNormalMap)
    {
        float3 normSample = objNormalMap.Sample(objSamplerState, input.texCord);
        
        input.normal.x = normSample.x * 2.0f - 1.0f;
        input.normal.y = normSample.y * 2.0f - 1.0f;
        input.normal.z = -normSample.z * 2.0f + 1.0f;
        
        input.normal = mul(float4(input.normal, 0.0f), worldMat);

    }
    
    LightingResult lit = ComputeLighting(input.worldPos, normalize(input.normal));

    float4 emissive = Material.Emissive;
    float4 ambient = Material.Ambient * float4(AmbientColor * AmbientIntensity, 1.0f);
    float4 diffuse = Material.Diffuse * lit.Diffuse;
    float4 specular = Material.Specular * lit.Specular;
    
    float4 pointColor;
    
    if(Material.UseTexture)
    {
        pointColor = objTexture.Sample(objSamplerState, input.texCord);
    }
    else
    {
        pointColor = input.color;
    }

    float4 finalColor = (ambient + diffuse) * pointColor + specular;
    return finalColor;
}