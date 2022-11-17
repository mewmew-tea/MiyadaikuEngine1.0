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

VSOut main( float4 pos : POSITION,
	float2 uv : TEXCOORD0,
	float4 col : COLOR
	)
{
    VSOut ret;
    ret.Pos = pos;
    //ret.Pos = mul(pos, g_mWorld);
    ret.Pos = mul(ret.Pos, g_mView); // view transformation
    ret.Pos = mul(ret.Pos, g_mProj); // projection transformation
    ret.UV = uv;
    ret.Color = col;
	
	return ret;
}
