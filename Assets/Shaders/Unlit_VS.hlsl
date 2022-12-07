struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

cbuffer cb7_Camera : register(b7)
{
    row_major float4x4 g_mView; // View matrix
    row_major float4x4 g_mProj; // Projection matrix
    row_major float4x4 g_mViewProj; // View matrix * Projection matrix

};


cbuffer Cb8_Uber : register(b8)
{
    row_major float4x4 g_mWorld; // world matrix
};

VSOut main( float4 pos : POSITION,
	float2 uv : TEXCOORD0,
	float4 col : COLOR,
    float3 normal : NORMAL,
    float3 tangent : TANGENT
	)
{
    VSOut ret;
    ret.Pos = pos;
    ret.Pos = mul(pos, g_mWorld);
    ret.Pos = mul(ret.Pos, g_mView);
    ret.Pos = mul(ret.Pos, g_mProj);
    ret.UV = uv;
    ret.Color = col;
    ret.Normal = normal;
    ret.Tangent = tangent;
	
	return ret;
}
