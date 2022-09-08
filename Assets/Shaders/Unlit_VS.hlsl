struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR;
};

VSOut main( float4 pos : POSITION,
	float2 uv : TEXCOORD0,
	float4 col : COLOR
	)
{
    VSOut ret;
    ret.Pos = pos;
    //ret.Pos = mul(pos, g_mWorld);
    //ret.Pos = mul(ret.Pos, g_mView);    // ƒrƒ…[•ÏŠ·
    //ret.Pos = mul(ret.Pos, g_mProj);    // Ë‰e•ÏŠ·
    ret.UV = uv;
    ret.Color = col;
	
	return ret;
}
