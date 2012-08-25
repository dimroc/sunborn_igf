//////////////////////////////////////////////////////////////////////
// Name		: FishEye.fx
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

float4x4 matMVP : WorldViewProjection;

texture image;
float2 PixelOffset;
float halfTexWidth;
float halfTexHeight;
float circRad;
float curve;

//////////////////////////////////////////////////////////////////////
// Samplers
//////////////////////////////////////////////////////////////////////

sampler FishEyeSampler = sampler_state
{
	Texture = (image);
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
	float xdist = inTex.x*halfTexWidth*2 - halfTexWidth;
	float ydist = inTex.y*halfTexHeight*2 - halfTexHeight;
	
	float dist = sqrt(xdist*xdist + ydist*ydist);
	
	float newX; 
	float newY; 
	float4 color;
	
	if(dist < circRad)
	{
		newX = inTex.x;
		newY = inTex.y;
		color = tex2D(FishEyeSampler, float2(newX, newY));
	}
	else
	{
		float ratio = (circRad + pow(dist-circRad, curve))/dist;
		newX = (ratio*xdist + halfTexWidth)*PixelOffset.x;
		newY = (ratio*ydist + halfTexHeight)*PixelOffset.y;
		color = tex2D(FishEyeSampler, float2(newX, newY));
	}
	
	
	return color;
}

//////////////////////////////////////////////////////////////////////
// Techniques
//////////////////////////////////////////////////////////////////////

technique Technique0
{
	pass Pass0
	{
		Sampler[0] = (FishEyeSampler);

		VertexShader = compile vs_2_0 vs_main();
		PixelShader  = compile ps_2_0 ps_main();
	}	
}