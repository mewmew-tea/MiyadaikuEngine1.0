struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

cbuffer cb0_Camera : register(b7)
{
    row_major float4x4 g_mView; // View matrix
    row_major float4x4 g_mProj; // Projection matrix
    row_major float4x4 g_mViewProj; // View matrix * Projection matrix

};

struct Material
{
    float4 BaseColor;
};

cbuffer cb1_Material : register(b1)
{
    Material g_Material;
};


struct Light
{
    float3 AmbientLight;
    float blank1;
    float3 DirectionalLightColor;
    float DirectionalLightIntensity;
    float3 DirectionalLightDir;
    float blank2;
};

cbuffer cb2_Light : register(b2)
{
    Light g_Light;
};

Texture2D g_baseColorTex : register(t0);
SamplerState g_ss : register(s0);

float4 main(VSOut In) : SV_TARGET
{
    
    return float4(g_baseColorTex.Sample(g_ss, In.UV).rgb, 1.0f) * g_Material.BaseColor;
}