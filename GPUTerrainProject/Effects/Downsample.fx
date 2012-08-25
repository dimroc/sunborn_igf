//////////////////////////////////////////////////////////////////////
// Name		: DownSample.fx
// Author	: Anirudh S Shastry. fantascape studios. Copyright © 2003.
// Date		: 28-12-2003, Sunday.
// Desc		: DownSampling shader.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float4x4 matMVP : WorldViewProjection;

texture tDownSample;

// Pixel offsets ( 1 / 1024, 1 / 768 )
float2 PixelOffset = float2( 0.00097656250, 0.0013020833 );

// This hack is necessary because the suppress function
// seems to work only when Kd is greater than 1, even by 0.0001f!!!
float Kd = 1.0001f;
float viewport_inv_width;
float viewport_inv_height;

//////////////////////////////////////////////////////////////////////
// Samplers
//////////////////////////////////////////////////////////////////////

sampler DownSampler = sampler_state
{
	Texture = (tDownSample);
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

float4 SuppressLDR( float4 c )
{
	float sum = c.r*c.g*c.b;
	sum = pow(sum, 3);
    return float4(c.rgb, sum)*sum;   

    return c;   		
}

float4 ps_main( float2 inTex: TEXCOORD0 ) : COLOR0
{
	float4 color = tex2D( DownSampler, inTex ) * Kd;
	return SuppressLDR( color );
}

//////////////////////////////////////////////////////////////////////
// Techniques
//////////////////////////////////////////////////////////////////////

technique Technique0
{
	pass Pass0
	{
		Sampler[0] = (DownSampler);

		VertexShader = compile vs_2_0 vs_main();
		PixelShader  = compile ps_2_0 ps_main();
	}
}