cbuffer cBuf : register(b0)
{
	float4x4 wvpMat;
	float4x4 worldMat;
};

cbuffer cLightMatrix : register(b1)
{
    float4x4 wvpLightMat;
}

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
    
    float4 lightViewPos : TEXCOORD1;
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
    
    float4 shadowPos = mul(float4(input.pos, 1.0f), worldMat);
    output.lightViewPos = mul(shadowPos, wvpLightMat);
	
	return output;
}