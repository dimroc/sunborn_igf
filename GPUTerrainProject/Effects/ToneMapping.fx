//////////////////////////////////////////////////////////////////////
// Name		: ToneMapping.fx
// Author	: Anirudh S Shastry. fantascape studios. Copyright © 2003.
// Date		: 28-12-2003, Sunday.
// Desc		: Tone mapping shader.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float4x4 matMVP : WorldViewProjection;

// Pixel offsets ( 1 / 1024, 1 / 768 )
float2 PixelOffset   = float2( 0.00097656250, 0.0013020833 );

// Exposure Level
float fExposureLevel;

float viewport_inv_width;
float viewport_inv_height;

texture tFull;
texture tBlur;

//////////////////////////////////////////////////////////////////////
// Samplers
//////////////////////////////////////////////////////////////////////

sampler FullSampler = sampler_state
{
	Texture = (tFull);
	AddressU = CLAMP;
	AddressV = CLAMP;
};

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
	OUT.Pos = float4( inPos.x, inPos.y, 0, 1);

	// Output the texture coordinates
	OUT.Tex.x = 0.5 * ( 1 + inPos.x) + 2*viewport_inv_width;
	OUT.Tex.y = 0.5 * ( 1 - inPos.y) + 2*viewport_inv_height;

	return OUT;
}

//////////////////////////////////////////////////////////////////////
// Pixel shader
//////////////////////////////////////////////////////////////////////

float4 ps_main( float2 inTex: TEXCOORD0 ) : COLOR0
{
	
	float4 original = tex2D( FullSampler, inTex );
	float4 blur		= tex2D( BlurSampler, inTex );

	float weight = (blur.r + blur.g + blur.b)/3;
	float weight2 = (original.r + original.g + original.b)/3;
	
	float4 color;
	
	if(weight > weight2)
		color = lerp( original, blur, (weight-weight2)*2);
	else
		color = original;	
	return color;
}

//////////////////////////////////////////////////////////////////////
// Techniques
//////////////////////////////////////////////////////////////////////

technique Technique0
{
	pass Pass0
	{
		Sampler[0] = (FullSampler);
		Sampler[1] = (BlurSampler);

		VertexShader = compile vs_2_0 vs_main();
		PixelShader  = compile ps_2_0 ps_main();
	}
}