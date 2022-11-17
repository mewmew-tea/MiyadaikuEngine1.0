struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR;
};

cbuffer cb0_Camera : register(b7)
{
    row_major float4x4 g_mView; // View matrix
    row_major float4x4 g_mProj; // Projection matrix
    row_major float4x4 g_mViewProj; // View matrix * Projection matrix

};

Texture2D g_baseColorTex : register(t0);
SamplerState g_ss : register(s0);

float4 main(VSOut In) : SV_TARGET
{
    //return float4(g_baseColorTex.Sample(g_ss, In.UV).rgb, 1.0f);
    return float4(g_baseColorTex.Sample(g_ss, In.UV).rgb, 1.0f);
    
    
    //return float4(1, 1, 0, 1);
    //const float4 baseCol = float4(g_baseColorTex.Sample(g_ss, In.UV).rgb, 1.0f) * g_Material.BaseColor * In.Color;
    //const float4 baseCol = float4(1,1,1,1) * g_Material.BaseColor * In.Color;
    //return baseCol;
    //return baseCol;
}