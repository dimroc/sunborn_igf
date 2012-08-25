//
// Render standard meshes ( no heirarchy ). 
// 

#define		ZBIAS		0.01f

// Pixel offsets ( 1 / 1024, 1 / 768 ). This is set in OnResetDevice using backbuffer display format.
float2 fPixelOffset = float2( 0.00097656250, 0.0013020833 );

float4 vecLightDir = {0.0f, 0.0f, -1.0f, 1.0f};    //light Direction 
// float4 vecLightColor = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse
float fAmbient = 0.2f;
float fFarPlane = 9999.f;

// float4 MaterialAmbient : MATERIALAMBIENT = {0.1f, 0.1f, 0.1f, 1.0f};
// float4 MaterialDiffuse : MATERIALDIFFUSE = {0.8f, 0.8f, 0.8f, 1.0f};

float3 vecCameraPos;
float4x4    mWorld : WORLD;
float4x4    mViewProj : VIEWPROJECTION;

//
// Toon / Silhouette stuff
//
float fShades;				// number of color shades
float fMinShade;			// clamp to this darkest shade. 
float fMinShadowShade;
float3 vEdgeColor;			// hard edge color
float fThreshold = 0.2f;	// dot product threshold for determining if silhouette.

//
// Shadow Stuff
//
float4x4	mShadowMatrix;
texture ShadowMap;
sampler2D ShadowMapSampler = sampler_state 
{
   Texture = <ShadowMap>;
   MinFilter = Point;
   MagFilter = Point;
   MipFilter = None;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};


//
// normal texture lookup
//
texture Texture;
sampler2D TextureSampler = sampler_state 
{
   Texture = <Texture>;
   MinFilter = Point;
   MagFilter = Point;
   MipFilter = None;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float3  Normal          : NORMAL;
    float3  Tex             : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float2  Tex     : TEXCOORD0;
    float   fShade  : TEXCOORD1;
    float   fSilhouetteFactor : TEXCOORD2;
    float4  PosCopy : TEXCOORD3;
    float4  PosShadow: TEXCOORD4;
};

float GetShadowPercentage(float3 PosShadow)
{
	float d = ( PosShadow.z ) / fFarPlane; // add a constant zbias.
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


VS_OUTPUT VS(VS_INPUT i)
{
    VS_OUTPUT   OUT;

    OUT.Pos = mul(float4(i.Pos.xyz, 1.f), mWorld);
    
    // Get Shadow Texture coordinate
    OUT.PosShadow = mul(float4(OUT.Pos.xyz, 1.f), mShadowMatrix);
    OUT.PosShadow.xy = OUT.PosShadow.xy / OUT.PosShadow.w;
    OUT.PosShadow.x += fPixelOffset.x;	// shift pixel cause of directx going from top left corner to center of pixel
    OUT.PosShadow.y -= fPixelOffset.y;
	OUT.PosShadow.xy = (OUT.PosShadow.xy + 1.f) / 2.f;
	OUT.PosShadow.y = 1 - OUT.PosShadow.y;	

    // normalize normals
    float3 Normal = normalize(i.Normal);

    // copy the input texture coordinate through
    OUT.Tex  = i.Tex.xy;

    // Get shade texture coordinate for toon shading intensity.
    OUT.fShade = saturate( fAmbient + saturate(dot(Normal, vecLightDir)) + 0.0001f );

    // Figure out lookAt vector for silhouette detection
    float3 los = normalize(vecCameraPos.xyz - OUT.Pos.xyz);	// line of sight
    OUT.fSilhouetteFactor = saturate(dot(Normal, los));

    // transform position from world space into clip space.
    OUT.Pos = mul(float4(OUT.Pos.xyz, 1.0f), mViewProj);
    OUT.PosCopy = OUT.Pos;	// copy redundantly for shadow test.

    return OUT;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(VS_OUTPUT IN) : COLOR
{
	float4 color = tex2D(TextureSampler, IN.Tex);
	
	// Check if hard edge
	if(IN.fSilhouetteFactor <= fThreshold)
	{
		color = float4(vEdgeColor, color.w);
	}
	else
	{
	//	SelfShadowing
	
		float shadowedShade = 1 - GetShadowPercentage(IN.PosShadow.xyz);
		shadowedShade *= fShades;
		float shade = max( fMinShade, floor(shadowedShade)*IN.fShade );		
	
		// float shade = max( fMinShade, IN.fShade * fShades );	// non self shadowing
		IN.fShade = saturate( shade / fShades );

		color.xyz *= IN.fShade;		// 'simplify' color to toon color.
	}
	return color;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS_Shadow(VS_OUTPUT IN) : COLOR
{		
	float4 color = tex2D(TextureSampler, IN.Tex);

	if(color.w > 0.f)
	{
		float d = IN.PosCopy.z / fFarPlane; // normalized so between 0 and 1
		return float4(d, d, d, 1.f);
	}
	else
		return float4(1.f, 1.f, 1.f, 0.f);						
}


//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique CelShade
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}

technique Shadow
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS_Shadow();
    }
}