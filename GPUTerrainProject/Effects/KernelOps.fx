//////////////////////////////////////////////////////////////////////
// Name		: KernelOp.fx
// Author	: Roger Roach
// Desc		: Applies kernel to an image
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float3x3 Kernel;
texture  texImage;

float2 PixelOffset;

//////////////////////////////////////////////////////////////////////
// Samplers
//////////////////////////////////////////////////////////////////////

sampler texImageSampler = sampler_state
{
	Texture = (texImage);
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
	OUT.Tex.x = 0.5 * ( 1 + inPos.x + PixelOffset[0]);
	OUT.Tex.y = 0.5 * ( 1 - inPos.y + PixelOffset[1]);

	return OUT;
}

//////////////////////////////////////////////////////////////////////
// Pixel shader
//////////////////////////////////////////////////////////////////////
// making it a bit more general to deal with certain enhancement ops we might want to do
// as well as making edge detections better, but it should still work for the old stuff too
float4 ps_main( float2 inTex: TEXCOORD0 ) : COLOR0
{
	float4 color = tex2D(texImageSampler, inTex)*Kernel[1][1];

	// Sample pixels on either side
	float4 color2 = float4(0.0, 0.0, 0.0, 0.0);
	for(int i = -1; i < 2; i ++)
	{
		color2 += tex2D( texImageSampler, inTex + dot(PixelOffset, float2(i, -1) ) ) * Kernel[i+1][0];
	}
	
	color2 += tex2D( texImageSampler, inTex + dot(PixelOffset, float2(-1, 0) ) ) * Kernel[0][1];
	color2 += tex2D( texImageSampler, inTex + dot(PixelOffset, float2( 1,  0) ) )  * Kernel[2][1];
	
	for(int i = -1; i < 2; i ++)
	{
		color2 += tex2D(texImageSampler, inTex + dot(PixelOffset, float2(i, 1) ) ) * Kernel[i+1][2];
	}	

	return float4(color.xyz, 1.f) + abs(color2);
}

//////////////////////////////////////////////////////////////////////
// Techniques
//////////////////////////////////////////////////////////////////////

technique Technique0
{
	pass Pass0
	{
		Sampler[0] = (texImageSampler);

		VertexShader = compile vs_2_0 vs_main();
		PixelShader  = compile ps_2_0 ps_main();
	}	
}