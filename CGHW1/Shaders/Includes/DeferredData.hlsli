struct PS_OUT
{
    float4 Diffuse : SV_Target0;
    float4 Emissive : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Specular : SV_Target3;
    float4 WorldPos : SV_Target4;
};