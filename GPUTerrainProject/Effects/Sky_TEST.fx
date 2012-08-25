// -------------------------------------------------------------
// Renders Sky Sphere using skytone and normals
//
// Fixed so works properly on both ati and nvidia.
// NOTE: ATI cards suck
//
// -------------------------------------------------------------

uniform float4x4 matWorldViewProj;
uniform float4x4 matInvTransposeWorld;
uniform float3 vecLightDir;
uniform float fTimeOfDay;

uniform float4 fSunColor;
uniform float fPixelShadePower;
uniform float fVertexShadePower;
uniform float fVertexShadeMultiplier;

uniform float fSkyToneResolution = 16.f;	// the dimensions of square skytone map (16x16).
uniform float fSkyToneResolutionInv = 1.f / 16.f;	// the dimensions of square skytone map (16x16).

texture SkyTone;

sampler2D SkyToneSampler = sampler_state
{
   Texture = <SkyTone>;
   MinFilter = LINEAR;
   MagFilter = LINEAR;
   MipFilter = LINEAR;   
   AddressU  = Wrap;
   AddressV  = Clamp;
};

// -------------------------------------------------------------
// vertex shader output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos: POSITION;   
   float2 Tex : TEXCOORD0;  
   float SunShade : TEXCOORD1;   
};

// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------

VS_OUTPUT VS( float4 Pos: POSITION, 
              float3 Normal : NORMAL, // normalized
              float2 Tex : TEXCOORD0 )
{
   VS_OUTPUT Out = (VS_OUTPUT) 0;   
   
   Out.Pos = mul(Pos, matWorldViewProj); // transform Position   
   Out.Tex = Tex;
   
   /*
   Out.Tex.x = fTimeOfDay;
   Out.Tex.y = Tex.y;   
   
   // float3 Norm = normalize(mul(matInvTransposeWorld, Normal)); // transform vertices normal by it's world inverse   
   
   Out.SunShade = saturate(dot(vecLightDir, -Normal));
   Out.SunShade = pow(Out.SunShade, fVertexShadePower);   // Exponentionally raised to create sun.         
   Out.SunShade *= fVertexShadeMultiplier;
   */
   
   return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(VS_OUTPUT IN) : COLOR
{   
/*
   // ATI, you suck.
   float2 Tex = IN.Tex;
   Tex.y *= fSkyToneResolution;	// clamp to some multiple of 1/16.   
   Tex.y = floor(Tex.y);
   Tex.y *= fSkyToneResolutionInv;   
   
   float3 ColorClamped = tex2D(SkyToneSampler, Tex);      
   Tex.y += fSkyToneResolutionInv;
   float3 ColorNext = tex2D(SkyToneSampler, Tex); 

   float tween = Tex.y - IN.Tex.y;
   float3 color = { lerp(ColorClamped.x, ColorNext.x, tween),
					lerp(ColorClamped.y, ColorNext.y, tween),
					lerp(ColorClamped.z, ColorNext.z, tween) };
	
   // color = float3( 0.f, 0.f, 0.f );
   
   IN.SunShade = pow(IN.SunShade, fPixelShadePower);
   // color = color + IN.SunShade * fSunColor;
   
   return float4(color,1.f);
*/ 

	return tex2D(SkyToneSampler, IN.Tex);
}
// -------------------------------------------------------------
// Name technique | pass | how to compile 
// -------------------------------------------------------------
technique SkyRender
{
    pass P0
    {
        Sampler[0] = (SkyToneSampler);
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
