// -------------------------------------------------------------
// Copy of the Fixed Function Pipeline that does vertex lighting
// Ambient Diffuse Specular Lighting
// -------------------------------------------------------------

float4x4 matWorldViewProj;	
float4x4 matInvTransposeWorld;
float4x4 matWorld;
float4x4 matViewInverse;


float4 vecLightDir;
float4 vecLightColor = {1.0f, 1.0f, 1.0f, 1.0f};

float4 lightAmbient = {0.2f, 0.2f, 0.2f, 1.0f};
float4 materialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
float4 materialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
float shininess = 30.0;

texture Texture;

sampler2D TextureSampler= sampler_state
{
   Texture = <Texture>;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};


// -------------------------------------------------------------
// vertex shader output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos: POSITION;   
   float2 Tex : TEXCOORD0;
   float4 DiffAmb : COLOR0;
   float4 Spec : COLOR1;
};

// -------------------------------------------------------------
// vertex shader function (input channels)
// -------------------------------------------------------------

VS_OUTPUT VS( float4 Pos: POSITION, 
              float3 Normal : NORMAL, // normalized
              float2 Tex : TEXCOORD0 )
{
   VS_OUTPUT Out = (VS_OUTPUT) 0;   
     
   float3 worldEyePos = matViewInverse[3].xyz;
   float3 worldVertPos = mul(Pos, matWorld).xyz;
   
   float4 Norm = normalize(mul(matInvTransposeWorld, Normal)); // transform vertices normal by it's world inverse
   float3 E = normalize(worldEyePos - worldVertPos); //eye vector
   float3 L = normalize( -vecLightDir.xyz); //light vector
   float3 H = normalize(E + L); //half angle vector
   
   //calculate the diffuse and specular contributions
   float  diff = max(0 , dot(Norm,L));
   float  spec = pow( max(0 , dot(Norm,H) ) , shininess );
   if( diff <= 0 )
   {
       spec = 0;
   }

   //output diffuse
   float4 ambColor = materialDiffuse * lightAmbient;
   float4 diffColor = materialDiffuse * diff * vecLightColor ;
   Out.DiffAmb = diffColor + ambColor;

   //output specular
   float4 specColor = materialSpecular * vecLightColor * spec;
   Out.Spec = specColor;

   return Out;
}

// -------------------------------------------------------------
// Pixel Shader (input channels):output channel
// -------------------------------------------------------------
float4 PS(float2 Tex: TEXCOORD0, float4 DiffAmb: COLOR0, float4 Spec: COLOR1) : COLOR
{   
   // Layer maps are tiled
   float4 c = tex2D(TextureSampler, Tex);  
   return DiffAmb * c + Spec;    
}

// -------------------------------------------------------------
// Name technique | pass | how to compile 
// -------------------------------------------------------------
technique AmbDifSpec
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
