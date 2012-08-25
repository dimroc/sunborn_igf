//////////////////////////////////////////////////////////////////////
// Name		: Blur.fx
// Author	: Anirudh S Shastry. fantascape studios. Copyright © 2003.
// Date		: 28-12-2003, Sunday.
// Desc		: Gaussian filter to blur along y-axis.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float4x4 matMVP : WorldViewProjection;

texture tBlur;
// Pixel offsets ( 1 / 512, 1 / 384 )
float2 PixelOffset;
float2 BlurOffset;
float PixelWeight[4] = { 0.25, 0.25, 0.25, 0.25};


//////////////////////////////////////////////////////////////////////
// Samplers
//////////////////////////////////////////////////////////////////////

sampler BlurSampler = sampler_state
{
	Texture = (tBlur);
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//////////////////////////////////////////////////////////////////////
// Vertex shader
//////////////////////////////////////////////////////////////////////

struct VS_OUT
{
	float4 Pos:	POSITION;
	float2 Tex:	TEXCOORD0;
};

VS_OUT vs_main( float3 inPos: POSITION, float2 inTex: TEXCOORD0 )
{
	VS_OUT OUT;

	// Output the transformed vertex
	OUT.Pos = float4( inPos.xy, 0, 1);

	// Output the texture coordinates
	OUT.Tex.x = 0.5 * ( 1 + inPos.x - PixelOffset.x);
	OUT.Tex.y = 0.5 * ( 1 - inPos.y - PixelOffset.y);

	return OUT;
}

//////////////////////////////////////////////////////////////////////
// Pixel shader
//////////////////////////////////////////////////////////////////////

float4 ps_main( float2 inTex: TEXCOORD0 ) : COLOR0
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sample pixels on either side
	for( int i = -1; i < 3; ++i )
	{
		color += tex2D( BlurSampler, inTex + ( BlurOffset * i ) ) * PixelWeight[i+1];
		color += tex2D( BlurSampler, inTex - ( BlurOffset * i ) ) * PixelWeight[i+1];
	}

	return color/8.f;
}

//////////////////////////////////////////////////////////////////////
// Techniques
//////////////////////////////////////////////////////////////////////

technique Technique0
{
	pass Pass0
	{
		Sampler[0] = (BlurSampler);

		VertexShader = compile vs_2_0 vs_main();
		PixelShader  = compile ps_2_0 ps_main();
	}	
}