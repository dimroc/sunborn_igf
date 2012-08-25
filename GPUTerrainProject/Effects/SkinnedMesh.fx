//
// Based on Skinned Mesh Effect file 
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
// EDITED: Dimitri Roche

#define		ZBIAS		0.01f

// Pixel offsets ( 1 / 1024, 1 / 768 ). This is also set in OnResetDevice.
float2 fPixelOffset = float2( 0.00097656250, 0.0013020833);

float4 vecLightDir = {0.0f, 0.0f, -1.0f, 1.0f};    //light Direction 
// float4 vecLightColor = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse
float fAmbient = 0.2f;
float fFarPlane = 9999.f;
// float4 MaterialAmbient : MATERIALAMBIENT = {0.1f, 0.1f, 0.1f, 1.0f};
// float4 MaterialDiffuse : MATERIALDIFFUSE = {0.8f, 0.8f, 0.8f, 1.0f};

float3 vecCameraPos;
// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
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
   AddressU  = Wrap;
   AddressV  = Wrap;
};

///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
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
	float d = ( PosShadow.z ) / fFarPlane - ZBIAS; // add a constant zbias.
	
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


float3 Diffuse(float3 Normal)
{
    float CosTheta;
    
    // N.L Clamped
    CosTheta = max(0.0f, dot(Normal, vecLightDir.xyz));
       
    // propogate scalar result to vector
    return (CosTheta);
}


VS_OUTPUT VShade(VS_INPUT i, uniform int NumBones)
{
    VS_OUTPUT	OUT = (VS_OUTPUT) 0;  
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])i.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);
    Normal += (mul(i.Normal, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); 
    
    //
    // Get Shadow Texture coordinate
    //
    OUT.PosShadow = mul(float4(Pos.xyz, 1.f), mShadowMatrix);
    OUT.PosShadow.xy = OUT.PosShadow.xy / OUT.PosShadow.w;
    OUT.PosShadow.x += fPixelOffset.x;	// shift pixel cause of directx going from top left corner to center of pixel
    OUT.PosShadow.y -= fPixelOffset.y;
	OUT.PosShadow.xy = (OUT.PosShadow.xy + 1.f) / 2.f;
	OUT.PosShadow.y = 1 - OUT.PosShadow.y;	
	
    // transform position from world space into view and then projection space
    OUT.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);
    OUT.PosCopy = OUT.Pos;

    // normalize normals
    Normal = normalize(Normal);

    // copy the input texture coordinate through
    OUT.Tex  = i.Tex.xy;
    
    // Get shade texture coordinate for toon shading intensity.
    OUT.fShade = saturate(fAmbient + saturate(dot(Normal, vecLightDir)) + 0.0001f );

    // Figure out lookAt vector for silhouette detection
    float3 los = normalize(vecCameraPos.xyz - Pos.xyz);	// line of sight
    OUT.fSilhouetteFactor = saturate(dot(Normal, los));

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
		float d = IN.PosCopy.z / fFarPlane; // need to normalize this to be between 0-1.
		return float4(d, d, d, 1.f);
	}
	else
		return float4(1.f, 1.f, 1.f, 0.f);						
}


int CurNumBones = 2;
VertexShader vsArray[4] = { compile vs_1_1 VShade(1), 
                            compile vs_1_1 VShade(2),
                            compile vs_1_1 VShade(3),
                            compile vs_1_1 VShade(4)
                          };

//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique CelShade
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
        PixelShader  = compile ps_2_0 PS();
    }
}

technique Shadow
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
        PixelShader  = compile ps_2_0 PS_Shadow();
    }
}

