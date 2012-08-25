// -------------------------------------------------------------
// Renders Sky Sphere using skytone and normals
//
// TODO: Fix up this stupid Shade hack and make a proper _SUN SHADER_
//
// -------------------------------------------------------------

uniform float4x4 matWorldViewProj;
uniform float4x4 matInvTransposeWorld;
uniform float3 vecLightDir;
uniform float fTimeOfDay;

uniform float4 fSunColor1, fSunColor2;
uniform float fPixelShadePower1, fPixelShadePower2;
uniform float fVertexShadePower1, fVertexShadePower2;
uniform float fVertexShadeMultiplier1, fVertexShadeMultiplier2;

texture SkyTone;

sampler2D SkyToneSampler= sampler_state
{
   Texture = <SkyTone>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = None;   
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
   float2 SunShade : TEXCOORD1;
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
   Out.Tex.x = fTimeOfDay;
   Out.Tex.y = Tex.y;   
   
   // float3 Norm = normalize(mul(matInvTransposeWorld, Normal)); // transform vertices normal by it's world inverse   
   
   // proper day sun
   Out.SunShade.x = saturate(dot(vecLightDir, Normal));   
   Out.SunShade.x = pow(Out.SunShade.x, fVertexShadePower1);   // Exponentionally raised to create sun.         
   Out.SunShade.x *= fVertexShadeMultiplier1;
   
   // edge night 'sun' moon wtever
   Out.SunShade.y = saturate(dot(-vecLightDir, Normal));
   Out.SunShade.y = pow(Out.SunShade.y, fVertexShadePower2);   // Exponentionally raised to create sun.         
   Out.SunShade.y *= fVertexShadeMultiplier2;

   return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(float2 Tex: TEXCOORD0, float2 SunShade : TEXCOORD1) : COLOR
{
   float3 color = tex2D(SkyToneSampler, Tex);   
   SunShade.x = pow(SunShade.x, fPixelShadePower1);
   SunShade.y = pow(SunShade.y, fPixelShadePower2);
   
   color = color + SunShade.x * fSunColor1 + SunShade.y * fSunColor2;
   return float4(color,1.f);
}
// -------------------------------------------------------------
// Name technique | pass | how to compile 
// -------------------------------------------------------------
technique SkyRender
{
    pass P0
    {
        //Sampler[0] = (BlendMapSampler);
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
