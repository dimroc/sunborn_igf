

float		fFarPlane = 9999.f;

float4x4    mWorld : WORLD;
float4x4    mViewProj : VIEWPROJECTION;

//
// normal texture lookup
//
texture Texture;
sampler2D TextureSampler = sampler_state 
{
   Texture = <Texture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = None;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};


///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float3  Tex             : TEXCOORD0;
};


struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float2  Tex     : TEXCOORD0;
    float4  PosCopy : TEXCOORD1;
};


VS_OUTPUT VS(VS_INPUT i)
{
    VS_OUTPUT   OUT;

    OUT.Pos = mul(float4(i.Pos.xyz, 1.f), mWorld);	// object space to world space
    
    // copy the input texture coordinate through
    OUT.Tex  = i.Tex.xy;

    // transform position from world space into clip space.
    OUT.Pos = mul(float4(OUT.Pos.xyz, 1.0f), mViewProj);
    OUT.PosCopy = OUT.Pos;	// copy redundantly for shadow test.

    return OUT;
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
	{		
		color.rgb = 1.f;
		color.a = 0.f;
		return color;
	}
}


//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique Shadow
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS_Shadow();
        
        AlphaTestEnable = true;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        ZEnable = false;
        ZWriteEnable = false;
    }
}