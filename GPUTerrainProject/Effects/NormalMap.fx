// -------------------------------------------------------------
// Emulates Refract light by displacing the passed texture 
// by the passed in normal map
// -------------------------------------------------------------

uniform float4x4	matWorldViewProj;	
uniform float4x4	matWorldView;
uniform float4x4	matView;
// float4x4	matInvTransposeWorld;	

uniform float4		vecLightDir;
uniform float4		vecLightPos;

uniform float2		fTiles;	//	texture tiling factor.

float3 ambient = {0.2,0.2,0.2};
float3 diffuse = {1,1,1};
float3 specular = {0.75,0.75,0.75};
float shine = 128;

texture		Texture;
texture		NormalMap;

sampler2D TextureSampler = sampler_state
{
   Texture = <Texture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D NormalMapSampler = sampler_state
{
   Texture = <NormalMap>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;
   AddressU  = Wrap;
   AddressV  = Wrap;
};


// -------------------------------------------------------------
// vertex shader output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 screenPos : POSITION;   
   float2 Tex : TEXCOORD0;
   float3 viewPos : TEXCOORD1;
   float3 tangent : TEXCOORD2;
   float3 binormal : TEXCOORD3;
   float3 normal : TEXCOORD4;
   float4 lightPos : TEXCOORD5;
};

// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------

VS_OUTPUT VS( float4 Pos: POSITION, 
              float3 Normal : NORMAL, // normalized              
              float2 Tex : TEXCOORD0,
              float3 Tangent: TANGENT0,
              float3 BiNormal: BINORMAL0 )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;   

    // vertex position in screen space
	Out.screenPos = mul( Pos, matWorldViewProj );

	// compute modelview rotation only part
	float3x3 worldViewRot;
	worldViewRot[0] = matWorldView[0].xyz;
	worldViewRot[1] = matWorldView[1].xyz;
	worldViewRot[2] = matWorldView[2].xyz;	

	// vertex position in view space (with model transformations)
	Out.viewPos = mul(Pos, matWorldView).xyz;

	// light position in view space
	float4 lp = float4(vecLightPos.x, vecLightPos.y, vecLightPos.z, 1.f);
	Out.lightPos = mul(lp, matView);

	// tangent space vectors in view space (with model transformations)
	Out.tangent = mul(Tangent, worldViewRot);
	Out.binormal = mul(BiNormal, worldViewRot);
	Out.normal = mul(Normal, worldViewRot);

	// copy texture coordinates
	Out.Tex = Tex;

	return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(VS_OUTPUT IN) : COLOR
{
    float4 normal = tex2D(NormalMapSampler, IN.Tex * fTiles);
    
	normal.xy = normal.xy*2.0-1.0; // transform to [-1,1] range
	normal.z = sqrt(1.0-dot(normal.xy,normal.xy)); // compute z component
	
	// transform normal to world space
	normal.xyz = normalize(normal.x * IN.tangent - normal.y * IN.binormal + normal.z * IN.normal);
	
	// color map
	float4 color = tex2D(TextureSampler, IN.Tex * fTiles);

	// view and light directions
	float3 v = normalize(IN.viewPos);
	float3 l = normalize(IN.lightPos.xyz - IN.viewPos);

	// compute diffuse and specular terms
	float att = saturate(dot(l,IN.normal.xyz));
	float diff = saturate(dot(l,normal.xyz));
	float spec = saturate(dot(normalize(l-v),normal.xyz));

	// compute final color
	float4 finalcolor;
	finalcolor.xyz = ambient*color.xyz +
					 att*(color.xyz * diffuse.xyz * diff + specular.xyz * pow(spec,shine));					 
	finalcolor.w = color.w;
	return finalcolor;
}

// -------------------------------------------------------------
// Name technique | pass | how to compile 
// -------------------------------------------------------------
technique NormalMap
{
    pass P0
    {
        // Sampler[0] = (BackGroundSampler);
		// Sampler[1] = (NormalMapSampler);
		
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
