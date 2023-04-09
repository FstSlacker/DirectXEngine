cbuffer cBuf : register(b0)
{
	float4x4 wvpMat;
	float4x4 worldMat;
};

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
	
	output.pos = mul(float4(input.pos, 1.0f), wvpMat);
    output.color = input.color;
	output.texCord = input.texCord;
    output.normal = mul(input.normal, (float3x3)worldMat);
    output.worldPos = mul(float4(input.pos, 1.0f), worldMat);
    output.tangent = mul(input.tangent, (float3x3)worldMat);
    output.bitangent = mul(input.bitangent, (float3x3)worldMat);
	
	return output;
}

struct PS_OUT
{
    float4 Diffuse : SV_Target0; //a - SpecularMap
    float4 Emissive : SV_Target1;
    float4 Normal : SV_Target2;
    float4 WorldPos : SV_Target3;
};

PS_OUT PSMain(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    output.Diffuse = DiffuseMap.Sample(Sampler, input.texCord);
    output.Diffuse.a = SpecularMap.Sample(Sampler, input.texCord).r;
    output.Emissive = input.color;
    output.WorldPos = float4(input.worldPos, 1.0f);
    output.Normal = float4(normalize(input.normal), 1.0f);
    
    return output;
}