struct PS_IN
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

PS_IN VSMain(uint id : SV_VertexID)
{
    PS_IN output = (PS_IN) 0;
    
    output.texCoord = float2(id & 1, (id & 2) >> 1);
    output.position = float4(output.texCoord * float2(2, -2) + float2(-1, 1), 0, 1);
    
    return output;
}