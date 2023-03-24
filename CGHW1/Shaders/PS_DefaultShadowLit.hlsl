#define MAX_LIGHTS 8

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define USE_DIFFUSE_TEXTURE 1
#define USE_NORMAL_MAP 2
#define USE_SPECULAR_MAP 4

Texture2D objTexture : register(t0);
Texture2D objNormalMap : register(t1);
Texture2D objSpecularMap : register(t2);

Texture2D shadowMapTex1 : register(t3);

SamplerState objSamplerState : register(s0);

SamplerState objSamplerTypeClamp : register(s1);

struct MaterialData
{
    float4 Emissive; // 16
    float4 Ambient; // 16
    float4 Diffuse; // 16
    float4 Specular; // 16
    
    int Flags;
    //float2 Padding; // 8
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
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    
    float4 lightViewPos : TEXCOORD1;
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

    return light.Color * pow(rDotV, Material.Specular.a) * light.Intensity;
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

bool IsShaded(float4 lightViewPosition)
{
    float bias;
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;


	// Set the bias value for fixing the floating point precision issues.
    bias = 0.001f;

	// Calculate the projected texture coordinates.
    projectTexCoord.x = (lightViewPosition.x / lightViewPosition.w) * 0.5f + 0.5f;
    projectTexCoord.y = -(lightViewPosition.y / lightViewPosition.w) * 0.5f + 0.5f;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
        depthValue = shadowMapTex1.Sample(objSamplerTypeClamp, projectTexCoord).r;

		// Calculate the depth of the light.
        lightDepthValue = (lightViewPosition.z / lightViewPosition.w);

		// Subtract the bias from the lightDepthValue.
        lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
        if (lightDepthValue < depthValue)
        {
            return false;
        }
        return true;
    }
    
    return false;
}

LightingResult ComputeLighting(float3 vertPos, float3 normal, float4 lightViewPos)
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
        
        if (IsShaded(lightViewPos))
            break;

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
    float4 pointColor;
    float4 specularWeight = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    if ((Material.Flags & USE_DIFFUSE_TEXTURE) != 0)
    {
        pointColor = objTexture.Sample(objSamplerState, input.texCord);
    }
    else
    {
        pointColor = input.color;
    }
    
    if ((Material.Flags & USE_NORMAL_MAP) != 0)
    {
        float3 normSample = objNormalMap.Sample(objSamplerState, input.texCord);
        
        const float3x3 tangSpaceRotMat = float3x3(
            input.tangent,
            input.bitangent,
            input.normal
        );
        
        input.normal.x = normSample.x * 2.0f - 1.0f;
        input.normal.y = -normSample.y * 2.0f + 1.0f;
        input.normal.z = -normSample.z;
        
        input.normal = mul(input.normal, tangSpaceRotMat);
    }
    
    if ((Material.Flags & USE_SPECULAR_MAP) != 0)
    {
        specularWeight = objSpecularMap.Sample(objSamplerState, input.texCord);
    }
    
    LightingResult lit = ComputeLighting(input.worldPos, normalize(input.normal), input.lightViewPos);

    float4 emissive = Material.Emissive;
    float4 ambient = Material.Ambient * float4(AmbientColor * AmbientIntensity, 1.0f);
    float4 diffuse = Material.Diffuse * lit.Diffuse;
    float4 specular = Material.Specular * lit.Specular * specularWeight;

    float4 finalColor = (ambient + diffuse) * pointColor + specular;
    return finalColor;
}