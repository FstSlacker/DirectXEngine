Texture2D objTexture : register(t0);
SamplerState objSamplerState : register(s0);

struct MaterialData
{
    float4 Emissive; // 16
    float4 Ambient; // 16
    float4 Diffuse; // 16
    float4 Specular; // 16
    
    float SpecularPower; // 4
    bool UseTexture; // 4
    //float2 Padding; // 8
};

cbuffer cMaterialBuf : register(b0)
{
    MaterialData Material;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
    float4 color : COLOR;
 	float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 pointColor;
    
    if(Material.UseTexture)
    {
        pointColor = objTexture.Sample(objSamplerState, input.texCord);
    }
    else
    {
        pointColor = input.color;
    }

    return pointColor;
}