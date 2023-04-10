#define USE_DIFFUSE_TEXTURE 1
#define USE_NORMAL_MAP 2
#define USE_SPECULAR_MAP 4

struct MaterialData
{
    float4 Emissive;
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    
    int Flags;
};

cbuffer cBuf : register(b0)
{
	float4x4 WorldViewProj;
	float4x4 World;
    float4 ViewPos;
};

cbuffer cMaterial : register(b1)
{
    MaterialData Material;
}

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);

SamplerState Sampler : register(s0);

struct VS_IN
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
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
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    output.pos = mul(float4(input.pos, 1.0f), WorldViewProj);
    output.color = input.color;
	output.texCord = input.texCord;
    output.normal = mul(input.normal, (float3x3)World);
    output.worldPos = mul(float4(input.pos, 1.0f), World);
    output.tangent = mul(input.tangent, (float3x3)World);
    output.bitangent = mul(input.bitangent, (float3x3)World);
	
	return output;
}

struct PS_OUT
{
    float4 Diffuse : SV_Target0; //a - SpecularMap
    float4 Emissive : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Specular : SV_Target3;
    float4 WorldPos : SV_Target4;
};

PS_OUT PSMain(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 specularColor = float4(Material.Specular.xyz, 1.0f);
    float4 diffuseColor = float4(Material.Diffuse.xyz, 1.0f);
    
    if ((Material.Flags & USE_DIFFUSE_TEXTURE) != 0)
    {
        diffuseColor *= DiffuseMap.Sample(Sampler, input.texCord);
    }
    
    if ((Material.Flags & USE_NORMAL_MAP) != 0)
    {
        float3 normSample = NormalMap.Sample(Sampler, input.texCord);
        
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
        specularColor *= SpecularMap.Sample(Sampler, input.texCord);
    }
    
    output.Diffuse = diffuseColor;
    output.Emissive = input.color;
    output.Normal = float4(normalize(input.normal), 1.0f);
    output.Specular = float4(specularColor.xyz, Material.Specular.a);
    output.WorldPos = float4(input.worldPos, 1.0f);
    
    return output;
}