// -------------------------------------------------------------
// Ambient and Diffuse Lighting
// Tiles 3 textures based on RGB components of BlendMap
// -------------------------------------------------------------

#define		ZBIAS		0.0f

// Pixel offsets ( 1 / 1024, 1 / 768 ). This is set in OnResetDevice using backbuffer display format.
float2 fPixelOffset = float2( 0.00097656250 / 2, 0.0013020833 / 2 );	// we use a shadow map DOUBLE the size of screen
float fFarPlane = 6999.f;

float4x4 mWorld;
float4x4 mViewProj;
float4x4 mITWorld;
float4x4 mShadowMatrix;

float4 vecLightDir;
float4 vecLightColor; // TODO:

float3 Aintensity = { 0.4f, 0.4f, 0.4f };
float3 Dintensity = { 0.8f, 0.8f, 0.8f };

float2 TWidthLength; // Terrain's width and length including cellspacing. Used to calc texture coordinates.
float2 Tween; 
float TextureScale;

texture BlendMap;
texture RedTexture;
texture GreenTexture;
texture BlueTexture;
texture AlphaTexture;
texture ShadowMap;

sampler2D BlendMapSampler= sampler_state
{
   Texture = <BlendMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

sampler2D RedTextureSampler= sampler_state
{
   Texture = <RedTexture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D GreenTextureSampler= sampler_state
{
   Texture = <GreenTexture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D BlueTextureSampler= sampler_state
{
   Texture = <BlueTexture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D AlphaTextureSampler= sampler_state
{
   Texture = <AlphaTexture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D ShadowMapSampler = sampler_state 
{
   Texture = <ShadowMap>;
   MinFilter = Point;
   MagFilter = Point;
   MipFilter = Linear;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

float GetShadowPercentage(float3 PosShadow)
{
	float d = ( PosShadow.z ) / fFarPlane; // add a constant zbias.
	
	/*
	// no PCF
	float4 shadow = tex2D(ShadowMapSampler, PosShadow.xy);
	if(d > shadow.x)
		return 1.f;
	else
		return 0.f;
	*/
	
    //
	// Use Percentage Closer Filtering to determine shadow amount
	//
	float2 spos;
	float4 shadow;
	float accumShadow = 0.f;
	for(int i=0; i<9; i++)
	{
		spos.x = PosShadow.x + fPixelOffset.x * ((i%3) - 1);	
		spos.y = PosShadow.y + fPixelOffset.y * (floor(i/3) - 1);
		shadow = tex2D(ShadowMapSampler, spos);
		if( d > shadow.x )
			accumShadow += 1.f;
	}
	accumShadow /= 9.f;	// percentage in shadow	
	return accumShadow;
}

// -------------------------------------------------------------
// vertex shader output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos: POSITION;
   float2 NonTiledTex : TEXCOORD0;
   float2 TiledTex : TEXCOORD1;
   float3 Shade : TEXCOORD2;
   float4 PosShadow : TEXCOORD3;
};

// -------------------------------------------------------------
// vertex shader function (input channels) - texture coordinates are calculated
// -------------------------------------------------------------

VS_OUTPUT VS( float3 Pos: POSITION, 
              float3 Normal : NORMAL, // normalized. invTransposeWorld is identity.              
              float2 Diff : TEXCOORD0 ) // morphing values
{
   VS_OUTPUT Out = (VS_OUTPUT) 0;      

   Pos.y += Diff.x * Tween.x;
   Pos.y += Diff.y * Tween.y;
   
   //
   // Texture coordinates
   //
   Out.NonTiledTex = Pos;	// vertex coordinates for terrain are as if always from 0,0 so we can do texturing.   
   Out.NonTiledTex.x = Pos.x / TWidthLength.x; // calculate texture coordinates. CellSpacing included.
   Out.NonTiledTex.y = (TWidthLength.y - Pos.z) / TWidthLength.y;
   Out.TiledTex = Out.NonTiledTex * TextureScale;
   
   Pos = mul(float4(Pos.xyz, 1.f), mWorld);
   //
   // Get Shadow Texture coordinate
   //
   Out.PosShadow = mul(float4(Pos.xyz, 1.f), mShadowMatrix);
   Out.PosShadow.xy = Out.PosShadow.xy / Out.PosShadow.w;
   Out.PosShadow.x += fPixelOffset.x;	// shift pixel cause of directx going from top left corner to center of pixel
   Out.PosShadow.y -= fPixelOffset.y;
   Out.PosShadow.xy = (Out.PosShadow.xy + 1.f) / 2.f;
   Out.PosShadow.y = 1 - Out.PosShadow.y;	
   
   Out.Pos = mul(float4(Pos, 1.0f), mViewProj);	// get screen space position.
   
   float4 Norm = normalize(mul(mITWorld, Normal)); // transform vertices normal by it's world inverse

/*
   Out.Shade = saturate(dot(-vecLightDir, Norm)) * Dintensity + Aintensity +
			   dot(-vecLightDir, float3(0.0, -1.0, 0.0))*float3(0.3, 0.3, 0.3);
*/
   Out.Shade = saturate(dot(-vecLightDir, Norm)) * Dintensity + Aintensity;

   return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(VS_OUTPUT IN) : COLOR
{   
   // Layer maps are tiled
   float3 cr = tex2D(RedTextureSampler, IN.TiledTex);
   float3 cg = tex2D(GreenTextureSampler, IN.TiledTex);
   float3 cb = tex2D(BlueTextureSampler, IN.TiledTex);
   float3 ca = tex2D(AlphaTextureSampler, IN.TiledTex);
    
   float4 blend = tex2D(BlendMapSampler, IN.NonTiledTex);
   
   // Find the inverse of all the blend weights so that we can
   // scale the total color to the range [0, 1].
   float totalInverse = 1.0f / (blend.r + blend.g + blend.b + blend.a);   

   cr *= blend.r * totalInverse;
   cg *= blend.g * totalInverse;
   cb *= blend.b * totalInverse;  
   ca *= blend.a * totalInverse;
   
   float shadow = GetShadowPercentage( IN.PosShadow.xyz );
   
   float3 final = (cr + cg + cb + ca) * max( IN.Shade - shadow, Aintensity );
   return float4(final, 1.0f);
}

// -------------------------------------------------------------
// Name technique | pass | how to compile 
// -------------------------------------------------------------
technique AmbientDif_MultiTex
{
    pass P0
    {
        //Sampler[0] = (BlendMapSampler);
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
        
        // Fog
        FogVertexMode	= LINEAR;
        FogStart		= 0.f;
        FogEnd			= 55000.f;
        
        FogColor		= 0x00cccccc;
        FogEnable		= false;
    }
}
