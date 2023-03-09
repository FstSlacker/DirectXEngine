cbuffer cBuf : register(b0)
{
	float4x4 wvpMat;
	float4x4 worldMat;
};

struct VS_IN
{
	float3 pos : POSITION0;
    float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float2 texCord : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : WORLD_POSITION;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos, 1.0f), wvpMat);
	output.texCord = input.texCord;
    output.normal = input.normal;
    output.worldPos = mul(float4(input.pos, 1.0f), worldMat);
	
	return output;
}