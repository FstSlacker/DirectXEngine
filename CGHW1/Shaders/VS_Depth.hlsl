cbuffer cBuf : register(b0)
{
    float4x4 wvpMat;
    float4x4 worldMat;
};

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
    float4 depthPosition : TEXTURE0;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN)0;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	// Store the position value in a second input value for depth value calculations.
    output.pos = mul(float4(input.pos, 1.0f), wvpMat);
    output.depthPosition = output.pos;
	
	return output;
}