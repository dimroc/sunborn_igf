#include "dxstdafx.h"
#include ".\Sky.h"
#include "gamedefine.h"
#include "globaldefine.h"


#define                 SKY_SPHERE_RESOLUTION                   70


CSky::CSky(const D3DXVECTOR3* pvCenter, float fRadius) :
    m_pSkyDomeVB(NULL), m_pSkyDomeIB(NULL), m_pSkyTone(NULL), 
    m_SkyEffect(NULL), m_SkyDecl(NULL),    
    m_Clock(0, 0.5f), m_bClockMoving(true)    
{
    memset(&m_OldSunStates, 0, sizeof(SSunMorphState)*2);
    memset(&m_bUpdateSun, 0, sizeof(bool)*2);
    memset(&m_fMorphTime, 0, sizeof(float)*2);
    m_bStartMorph[0] = m_bStartMorph[1] = true;
    m_CurrentSunStates[0] = SunStates::DefaultSun;
    m_CurrentSunStates[1] = SunStates::BlackEdgeMorph;

	SetWorldPosition(*pvCenter, fRadius);

    m_LightDir = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 0.0f); // shining down     
    m_LightRight = D3DXVECTOR3(0.f, 0.f, -1.f);
}

CSky::~CSky(void)
{
    SAFE_RELEASE(m_pSkyDomeVB);
    SAFE_RELEASE(m_pSkyDomeIB);
    SAFE_RELEASE(m_pSkyTone);

    for(size_t i = 0; i < m_vpBigStars.size(); i++) {
        SAFE_DELETE(m_vpBigStars[i]);
    }

    SAFE_RELEASE(m_SkyEffect);
    SAFE_RELEASE(m_SkyDecl);  
}

VOID CSky::SetWorldPosition(const D3DXVECTOR3& center, float radius)
{
    m_vWorldCenter = center;
    m_fSkyRadius = radius;

	D3DXMATRIX mtxTrans, mtxScale;
	D3DXMatrixTranslation(&mtxTrans, center.x, center.y, center.z);
	D3DXMatrixScaling(&mtxScale, radius, radius, radius);

	m_World = mtxScale * mtxTrans;

	D3DXMatrixTranspose(&m_InvTransWorld, &m_World);
	D3DXMatrixInverse(&m_InvTransWorld, NULL, &m_InvTransWorld);

    // Send to SBigStar to konw how/where to rotate around
    SBigStar::SetWorldOrbit(center, radius);
}

VOID CSky::OnCreateDevice(IDirect3DDevice9* pd3dDevice,
                          LPCSTR skytoneFilename)
{
    HRESULT hr;

    //
    // load sky tone, to be used to interpolate colors from zenith to horizon.
    //
    V( D3DXCreateTextureFromFileA( pd3dDevice, skytoneFilename, &m_pSkyTone ) );

    //
    // Create the shaders, techniques, etc for sky.
    //

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    V( D3DXCreateEffectFromFile( pd3dDevice, SKY_EFFECT_NAME, NULL, NULL, dwShaderFlags, 
                                        NULL, &m_SkyEffect, NULL ) );

    // Get Handles
    m_SkyTechnique = m_SkyEffect->GetTechniqueByName( SKY_TECHNIQUE_NAME );
    m_WorldViewProjHandle = m_SkyEffect->GetParameterByName(NULL, "matWorldViewProj");
    m_InvTransWorldHandle = m_SkyEffect->GetParameterByName(NULL, "matInvTransposeWorld");
    m_LightDirHandle = m_SkyEffect->GetParameterByName(NULL, "vecLightDir");
    m_TimeOfDayHandle = m_SkyEffect->GetParameterByName(NULL, "fTimeOfDay");

    // Sun Handles
    m_SunColorHandle[0] = m_SkyEffect->GetParameterByName(NULL, "fSunColor1");
    m_SunPixelShaderPowerHandle[0] = m_SkyEffect->GetParameterByName(NULL, "fPixelShadePower1");
    m_SunVertexShaderPowerHandle[0] = m_SkyEffect->GetParameterByName(NULL, "fVertexShadePower1");
    m_SunVertexShaderMultiplierHandle[0] = m_SkyEffect->GetParameterByName(NULL, "fVertexShadeMultiplier1");

    m_SunColorHandle[1] = m_SkyEffect->GetParameterByName(NULL, "fSunColor2");
    m_SunPixelShaderPowerHandle[1] = m_SkyEffect->GetParameterByName(NULL, "fPixelShadePower2");
    m_SunVertexShaderPowerHandle[1] = m_SkyEffect->GetParameterByName(NULL, "fVertexShadePower2");
    m_SunVertexShaderMultiplierHandle[1] = m_SkyEffect->GetParameterByName(NULL, "fVertexShadeMultiplier2");

    V( pd3dDevice->CreateVertexDeclaration( SkyVert::Decl, &m_SkyDecl ) );

    //
    // Prepare Effect
    //
    V( m_SkyEffect->SetTexture("SkyTone", m_pSkyTone) );
    V( m_SkyEffect->SetFloat(m_TimeOfDayHandle, 0.f) );    // Set Time

    V( m_SkyEffect->SetVector(m_SunColorHandle[0], &m_CurrentSunStates[0].color) );
    V( m_SkyEffect->SetFloat(m_SunPixelShaderPowerHandle[0], m_CurrentSunStates[0].pspower) );
    V( m_SkyEffect->SetFloat(m_SunVertexShaderPowerHandle[0], m_CurrentSunStates[0].vspower) );
    V( m_SkyEffect->SetFloat(m_SunVertexShaderMultiplierHandle[0], m_CurrentSunStates[0].vsmult) );

    V( m_SkyEffect->SetVector(m_SunColorHandle[1], &m_CurrentSunStates[1].color) );
    V( m_SkyEffect->SetFloat(m_SunPixelShaderPowerHandle[1], m_CurrentSunStates[1].pspower) );
    V( m_SkyEffect->SetFloat(m_SunVertexShaderPowerHandle[1], m_CurrentSunStates[1].vspower) );
    V( m_SkyEffect->SetFloat(m_SunVertexShaderMultiplierHandle[1], m_CurrentSunStates[1].vsmult) );
    
    //
    // call helper function to create skydome sphere.
    //
    CreateSphere(pd3dDevice, SKY_SPHERE_RESOLUTION, SKY_SPHERE_RESOLUTION);
}

VOID CSky::AddBigStar(IDirect3DDevice9* pd3dDevice, 
                    LPCWSTR texFileName,    // texture for star
                    float orbit_speed,  // speed star rotates around terrain
                    float angle,    // angle along south/north plane. (rotates from east to west)
                    float time_offset,  // the rotation position offset from noon(straight above) in time.
                    float xscale,
                    float zscale) 
{
	SBigStar* pStar = new SBigStar(
								pd3dDevice,
								texFileName,
								orbit_speed,// have to convert orbit speed to clock speed.								
								angle,
								time_offset*SClock::FULL_DAY,
								xscale,
								zscale);

    m_vpBigStars.push_back(pStar);
}

VOID CSky::OnFrameMove(float fElapsedTime, const D3DXMATRIX* pmViewProj)
{
    using namespace gdef;

    HRESULT hr;    

    // set worldviewproj in shader to transform vertex.
    D3DXMATRIX mtx = m_World * (*pmViewProj);

    V( m_SkyEffect->SetMatrix(m_WorldViewProjHandle, &mtx) );    
    V( m_SkyEffect->SetMatrix(m_InvTransWorldHandle, &m_InvTransWorld) );    

    // Move Clock    
    if(m_bClockMoving) {
        m_Clock.time += fElapsedTime * m_Clock.speed;
        if(m_Clock.time >= 24.0f) m_Clock.time -= 24.0f;
    }    

    // Move Stars
    for(size_t i=0; i<m_vpBigStars.size(); i++)
        m_vpBigStars[i]->OnFrameMove(m_Clock.time);

    // Get Light Space
    if(m_vpBigStars.size() > 0) {
        m_LightDir.x = m_vWorldCenter.x - m_vpBigStars[0]->lightPos.x;
        m_LightDir.y = m_vWorldCenter.y - m_vpBigStars[0]->lightPos.y;
        m_LightDir.z = m_vWorldCenter.z - m_vpBigStars[0]->lightPos.z;
        m_LightDir.w = 0.f;
        D3DXVec4Normalize(&m_LightDir, &m_LightDir);

        D3DXVec3Cross(&m_LightUp, (D3DXVECTOR3*)&m_LightDir, &m_LightRight);

        D3DXVECTOR4 vStarDir;
        vStarDir.x = m_vWorldCenter.x - m_vpBigStars[0]->pos.x;
        vStarDir.y = m_vWorldCenter.y - m_vpBigStars[0]->pos.y;
        vStarDir.z = m_vWorldCenter.z - m_vpBigStars[0]->pos.z;
        vStarDir.w = 0.f;
        D3DXVec4Normalize(&vStarDir, &vStarDir);
        V( m_SkyEffect->SetVector(m_LightDirHandle, &vStarDir) );
    }

    V( m_SkyEffect->SetFloat(m_TimeOfDayHandle, m_Clock.time / 24.0f) );    // Set Time

    for(int i=0; i<2; i++)  // there are 2 suns
    {
        if(!m_dSunStates[i].empty())
        {   // Deal with sun morph states. The speed of morphs are independent to clock time/speed.
            if(m_bStartMorph[i]) {
                m_OldSunStates[i] = m_CurrentSunStates[i];
                m_bStartMorph[i] = false;
            }
            deque<SSunMorphState>::reference ref = m_dSunStates[i].front();

            // Interpolate from old sun state to desired.
            assert(ref.fMorphTime > 0.f); // prevent division by 0
            float interp = m_fMorphTime[i] / ref.fMorphTime;
            m_CurrentSunStates[i].pspower = lerp(m_OldSunStates[i].pspower, ref.pspower, interp);
            m_CurrentSunStates[i].vspower = lerp(m_OldSunStates[i].vspower, ref.vspower, interp);
            m_CurrentSunStates[i].vsmult = lerp(m_OldSunStates[i].vsmult, ref.vsmult, interp);

            lerp(&m_CurrentSunStates[i].color, &m_OldSunStates[i].color, &ref.color, interp);        

            m_fMorphTime[i] += fElapsedTime;
            if(ref.fMorphTime < m_fMorphTime[i]) {
                m_dSunStates[i].pop_front();
                m_bStartMorph[i] = true;
                m_fMorphTime[i] = 0.f;
            }
            m_bUpdateSun[i] = true;
        }

        if(m_bUpdateSun[i]) { // update the sun(duh)        
            V( m_SkyEffect->SetVector(m_SunColorHandle[i], &m_CurrentSunStates[i].color) );
            V( m_SkyEffect->SetFloat(m_SunPixelShaderPowerHandle[i], m_CurrentSunStates[i].pspower) );
            V( m_SkyEffect->SetFloat(m_SunVertexShaderPowerHandle[i], m_CurrentSunStates[i].vspower) );
            V( m_SkyEffect->SetFloat(m_SunVertexShaderMultiplierHandle[i], m_CurrentSunStates[i].vsmult) );
            m_bUpdateSun[i] = false;
        }
    }
}

HRESULT CSky::OnFrameRender(IDirect3DDevice9 *pd3dDevice)
{   // TODO: Perhaps just Render the m_pBackgroundSkyTex to the buffer instead of rerendering.
    HRESULT hr;
    UINT cPass;     

    //
    // Render Sky
    //
    
    V( pd3dDevice->SetStreamSource(0, m_pSkyDomeVB, 0, sizeof(SkyVert)) );
    V( pd3dDevice->SetIndices(m_pSkyDomeIB) );    

    V( pd3dDevice->SetVertexDeclaration( m_SkyDecl ) );    

    V( m_SkyEffect->SetTechnique(m_SkyTechnique) );

    // Flip Cull Mode so you see inside sphere.
    V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW) );	

    V( m_SkyEffect->Begin(&cPass, 0) ); // Render through all passes
    for(UINT p=0; p<cPass; p++) 
    {
        V( m_SkyEffect->BeginPass( p ) );
        V( pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertices, 0, m_nSphereTriangles - 2) );
        V( m_SkyEffect->EndPass() );
    }
    V( m_SkyEffect->End() );        

    V( pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW) );

    //
    // Render Sun, Stars, etc.
    //

    // TODO: Batch Render by Texture    
    V( pd3dDevice->SetStreamSource(0, SBigStar::pVB, 0, sizeof(BoxVertTex)) );
    V( pd3dDevice->SetFVF(BoxVertTex::FVF) );

    V( pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE) );
    V( pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000004));
    V( pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) );
    V( pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) );

    // Texture blending operations within texture
    V( pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ) );
    V( pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 ) );

    V( pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ) );    
    V( pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1) );

    for(size_t i=1; i<m_vpBigStars.size(); i++)
        m_vpBigStars[i]->OnFrameRender(pd3dDevice);

    return S_OK;
}

HRESULT CSky::OnResetDevice(IDirect3DDevice9 * pd3dDevice)
{       
    if(m_SkyEffect)
        m_SkyEffect->OnResetDevice();
    return S_OK;
}

HRESULT CSky::OnLostDevice()
{
    if(m_SkyEffect)
        m_SkyEffect->OnLostDevice();
    return S_OK;
}

VOID CSky::SetLightDirByTimeOfDay(float fLightDirByTimeOfDay)
{
    assert(m_vpBigStars.size() > 0);

    m_vpBigStars[0]->fLightTimeOffset = fLightDirByTimeOfDay - m_Clock.time;

}

// --------------------------------------------------------------------------------------
// Private Functions
// --------------------------------------------------------------------------------------

//! Creates a sphere of radius 1
HRESULT CSky::CreateSphere(IDirect3DDevice9* pd3dDevice, int nRings, int nSegments)
{
    HRESULT hr;
    // set vertex count and index count 

    DWORD dwVertices = ( nRings + 1 ) * ( nSegments + 1 ) ;
    DWORD dwIndices = 2 * nRings * ( nSegments + 1 ) ;

    m_nVertices = dwVertices;
    m_nIndices = dwIndices;

    // Create the vertex buffer

    V_RETURN( pd3dDevice->CreateVertexBuffer( 
        dwVertices * sizeof ( SkyVert ), 
        D3DUSAGE_WRITEONLY, 0,  
        D3DPOOL_MANAGED, &m_pSkyDomeVB, NULL ) );

    // Create the index buffer

    V_RETURN( pd3dDevice->CreateIndexBuffer( 
        dwIndices * 2, 
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
        D3DPOOL_MANAGED, &m_pSkyDomeIB, NULL ) );        

    // Lock the vertex buffer

    SkyVert *pVertex ; 
    V_RETURN( m_pSkyDomeVB->Lock( 0, 0, (VOID**) &pVertex , 0 ) );

    // lock the index buffer 

    WORD *pIndices;
    V_RETURN( m_pSkyDomeIB->Lock( 0, 0, ( VOID ** ) &pIndices, 0 ) );

    // Establish constants used in sphere generation

    FLOAT fDeltaRingAngle = ( D3DX_PI / nRings );
    FLOAT fDeltaSegAngle = ( 2.0f * D3DX_PI / nSegments );

    WORD wVerticeIndex = 0;    
    int count =0;
    // Generate the group of rings for the sphere

    for( int ring = 0; ring < nRings + 1 ; ring++ )
    {
        FLOAT r0 = sinf ( ring * fDeltaRingAngle );
        FLOAT y0 = cosf ( ring * fDeltaRingAngle );

        // Generate the group of segments for the current ring

        for( int seg = 0; seg < nSegments + 1 ; seg++ )
        {
            FLOAT x0 = r0 * sinf( seg * fDeltaSegAngle );
            FLOAT z0 = r0 * cosf( seg * fDeltaSegAngle );

            // Add one vertices to the strip which makes up the sphere

            pVertex->pos = D3DXVECTOR3(x0,y0,z0); // normalized 

            // point normal inside sphere
            pVertex->norm = -pVertex->pos ; // since sphere is of radius 1, it's normalized.
            // pVertex->color = 0xffffffff ; 
            //pVertex->tu = (FLOAT) seg / (FLOAT) nSegments;
            pVertex->tu = 0.0f;
            pVertex->tv = (FLOAT) ring / ( (FLOAT) nRings / 2.0f );

            pVertex ++;            

            // add two indices except for last ring 

            if ( ring != nRings ) 
            {
                *pIndices = wVerticeIndex ;
                pIndices ++ ;
                *pIndices = wVerticeIndex + ( WORD ) ( nSegments + 1 ) ;
                pIndices ++ ;
                wVerticeIndex ++ ;

                count += 2;
            }
        } // end for seg 
    } // end for ring     

    m_nSphereTriangles = count;

    m_pSkyDomeIB->Unlock();
    m_pSkyDomeVB->Unlock();

    return S_OK ; 
}


// --------------------------------------------------------------------------------------
// BigStar,  like sun, moon ( I KNOW MOON NOT A STAR TSTUPDI)(R
// --------------------------------------------------------------------------------------

CSky::SBigStar::SBigStar(IDirect3DDevice9 *pd3dDevice, 
                         LPCWSTR filename, 
                         float _orbit_speed, 
                         float _angle, 
                         float _time_offset,
                         float xscale,
                         float zscale) :
    orbit_speed(_orbit_speed), angle(_angle), time_offset(_time_offset), pTex(NULL)
{
    HRESULT hr;
    V( D3DXCreateTextureFromFile(pd3dDevice, filename, &pTex) );

    // Create static Quad once
    if(!pVB)
        InitQuad(pd3dDevice);

	D3DXMatrixScaling(&mScale, xscale, 0, zscale);
    // Create initial pos. 
    start_pos.x = worldcenter.x - magnitude / 2.0f; 
	start_pos.y = 0.0f; 
	start_pos.z = worldcenter.z + magnitude * sin(angle);	
	
    alpha_multiplier = 1.0f / 24.0f * 2.0f * D3DX_PI * orbit_speed;
	angle_offset = time_offset / 24.0f * 2.0f * D3DX_PI;
    fLightTimeOffset = 0.f;
}

CSky::SBigStar::~SBigStar()
{
    SAFE_RELEASE(pTex);
    SAFE_RELEASE(pVB);
}

// delta-t * speed + time_offset = alpha. alpha determines rotation position about world center.
VOID CSky::SBigStar::OnFrameMove(float fTimeofDay)
{
	static float D3DX_PI_OVER2 = D3DX_PI / 2.0f;

    //deal with star position
    float alpha = fTimeofDay * alpha_multiplier + angle_offset;
	
	pos.x = worldcenter.x - magnitude * cos(alpha);
	pos.y = magnitude * sin(alpha);
	pos.z = start_pos.z;
    D3DXMatrixTranslation(&mWorld, pos.x, pos.y, pos.z);	

	// Rotate vertices to face worldcenter if using decorations and not actual shader sun.
	D3DXMATRIX mRot;
	D3DXMatrixRotationZ( &mRot, - alpha + D3DX_PI_OVER2 );

    mWorld = mScale * mRot * mWorld;

    //deal with star light position
    float light_alpha = (fTimeofDay + fLightTimeOffset) * alpha_multiplier + angle_offset;
	
	lightPos.x = worldcenter.x - magnitude * cos(light_alpha);
	lightPos.y = magnitude * sin(light_alpha);
	lightPos.z = start_pos.z;    
}

VOID CSky::SBigStar::OnFrameRender(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr;
    V( pd3dDevice->SetTransform( D3DTS_WORLD, &mWorld) );
    V( pd3dDevice->SetTexture(0, pTex) );

    V( pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2) );
}

// --------------------------------------------------------------------------------------
// BigStar's STATICS
// --------------------------------------------------------------------------------------

VOID CSky::SBigStar::InitQuad(IDirect3DDevice9* pd3dDevice)
{
    HRESULT hr;
    // Create the vertex buffer

    V( pd3dDevice->CreateVertexBuffer( 
        4 * sizeof ( BoxVertTex ), 
        D3DUSAGE_WRITEONLY, BoxVertTex::FVF,  
        D3DPOOL_MANAGED, &pVB, NULL ) );    

    // Lock the vertex buffer

    BoxVertTex *pVertex ; 
    V( pVB->Lock( 0, 0, (VOID**) &pVertex , 0 ) );

    // Make Quad

    pVertex[0] = BoxVertTex(D3DXVECTOR3(-1.0f, 0.0f, 1.0f), 0.0f, 1.0f);    
    pVertex[1] = BoxVertTex(D3DXVECTOR3(-1.0f, 0.0f, -1.0f), 0.0f, 0.0f);
    pVertex[2] = BoxVertTex(D3DXVECTOR3(1.0f, 0.0f, 1.0f), 1.0f, 1.0f);
    pVertex[3] = BoxVertTex(D3DXVECTOR3(1.0f, 0.0f, -1.0f), 1.0f, 0.0f);

    V( pVB->Unlock() );
}

VOID CSky::SBigStar::SetWorldOrbit(const D3DXVECTOR3& center, float rad)
{
    worldcenter = center;
    magnitude = rad;
}

// --------------------------------------------------------------------------------------
// CSKY STATICS
// --------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 CSky::SBigStar::pVB = NULL;
D3DXVECTOR3 CSky::SBigStar::worldcenter;
float CSky::SBigStar::magnitude = 0.0f;
float CSky::SClock::FULL_DAY = 24.0f;
