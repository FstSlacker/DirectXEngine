cbuffer cBuf : register(b0)
{
	float4x4 mat;
};

struct VS_IN
{
	float3 pos : POSITION0;
	float4 col : COLOR0;
    float2 texCord : TEXCOORD;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
 	float2 texCord : TEXCOORD;
};

Texture2D objTexture : register(t0);
SamplerState objSamplerState : register(s0);

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos, 1.0f), mat);
	output.col = input.col;
	output.texCord = input.texCord;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float3 pixelColor = objTexture.Sample(objSamplerState, input.texCord);
    return float4(pixelColor, 1.0f);
}