// -------------------------------------------------------------
// Renders Sky Sphere using skytone and normals
// -------------------------------------------------------------

float4x4 matWorldViewProj;
float3 vecLightDir;
float fObjectHeight;

float4 vecClusterTranslation; // Calculated on CPU

// -------------------------------------------------------------
texture GrassTexture;
sampler2D GrassTextureSampler= sampler_state
{
   Texture = <GrassTexture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};
// -------------------------------------------------------------

// -------------------------------------------------------------
// vertex shader output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos: POSITION;
   float4 Dif: COLOR;
   float2 Tex : TEXCOORD0;  
};

// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------

VS_OUTPUT VS( float4 Pos: POSITION, 
              float3 Normal : NORMAL, // normalized
              float2 Tex : TEXCOORD0)  
{
   VS_OUTPUT Out = (VS_OUTPUT) 0;
   
   if(Tex.y <= 0.1f)   // Only animates upper vertices
   {  
      Pos += vecClusterTranslation;
      Normal = normalize(Normal * fObjectHeight + vecClusterTranslation);      
   }
   
   Out.Pos = mul(Pos, matWorldViewProj); // transform Position
   Out.Dif = dot(vecLightDir, Normal);
   Out.Tex = Tex;      
   return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(float4 Dif: COLOR, float2 Tex: TEXCOORD0) : COLOR
{  
   float4 color = tex2D(GrassTextureSampler, Tex);
   color *= Dif;
   return color;
}

// -------------------------------------------------------------
// Name technique | pass | how to compile 
// -------------------------------------------------------------
technique GrassMeadowRender
{
    pass P0
    {
        //Sampler[0] = (BlendMapSampler);
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
        
        // Do no set sampler state in effect.
    }
}