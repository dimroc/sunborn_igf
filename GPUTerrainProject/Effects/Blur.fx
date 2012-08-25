//////////////////////////////////////////////////////////////////////
// Name		: Blur.fx
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float4x4 matMVP : WorldViewProjection;

texture tBlur;
// Pixel offsets ( 1 / 512, 1 / 384 )
float2 PixelOffset;
float2 BlurOffset;

//float PixelWeight[5] = { 0.0, 0.1, 0.35, 0.3, 0.25 };
float PixelWeight[8] = { 0.0, 0.1, 0.3, 0.2, 0.15, 0.1, 0.1, 0.05 };

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
	OUT.Tex.x = 0.5 * ( 1 + inPos.x);
	OUT.Tex.y = 0.5 * ( 1 - inPos.y);

	return OUT;
}

//////////////////////////////////////////////////////////////////////
// Pixel shader
//////////////////////////////////////////////////////////////////////

float4 ps_main( float2 inTex: TEXCOORD0 ) : COLOR0
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sample pixels on either side
	for( int i = 0; i < 8; ++i )
	{
		color += tex2D( BlurSampler, inTex + ( BlurOffset * i ) ) * PixelWeight[i];
		color += tex2D( BlurSampler, inTex - ( BlurOffset * i ) ) * PixelWeight[i];
	}

	return color*0.5;
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