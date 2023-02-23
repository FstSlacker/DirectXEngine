cbuffer cBuf : register(b0)
{
	float4x4 mat;
};

struct VS_IN
{
	float3 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos, 1.0f), mat);
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;

	float v = sqrt((0.5f - col.z) * (0.5f - col.z) + (0.5f - col.w) * (0.5f - col.w));
	if (v <= 0.5f)
	{
		return float4(col.x, col.y, 0.0f, 1.0f);
	}
	else
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}