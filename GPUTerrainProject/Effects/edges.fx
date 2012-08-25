//////////////////////////////////////////////////////////////////////
// Name		: edges.fx
// Author	: Roger Roach
// Desc		: Does an edge detection on an image given a threshold
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float	 thresh;
texture  texImage;

float2	 PixelOffset;
float	 color;
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
float4 ps_main( float2 inTex: TEXCOORD0 ) : COLOR0
{
	float2 PO = PixelOffset * (1 - color)/2;
	float4 horEdge = - tex2D(texImageSampler, inTex + float2(-PO[0], -PO[1])) +
					 - tex2D(texImageSampler, inTex + float2(     0, -PO[1])) +
					 - tex2D(texImageSampler, inTex + float2( PO[0], -PO[1])) +
					   tex2D(texImageSampler, inTex + float2(-PO[0],  PO[1])) +
					   tex2D(texImageSampler, inTex + float2(     0,  PO[1])) +
					   tex2D(texImageSampler, inTex + float2( PO[0],  PO[1]));
					   
	float4 verEdge = - tex2D(texImageSampler, inTex + float2(-PO[0], -PO[1])) +
					 - tex2D(texImageSampler, inTex + float2(-PO[0],      0)) +
					 - tex2D(texImageSampler, inTex + float2(-PO[0],  PO[1])) +
					   tex2D(texImageSampler, inTex + float2( PO[0], -PO[1])) +
					   tex2D(texImageSampler, inTex + float2( PO[0],      0)) +
					   tex2D(texImageSampler, inTex + float2( PO[0], -PO[1]));
		
	float4 edge = abs(horEdge) + abs(verEdge);
	float sum = edge.x + edge.y + edge.z + edge.w;
	
	float4 norm =tex2D(texImageSampler, inTex);	
	float val = 0.3*norm.x + 0.59*norm.y + 0.11*norm.z;	
	
	edge = (sum < thresh) * float4(val, val, val, 1.0);
	
	return lerp(float4(edge.xyz, 1.0), norm, color);	
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