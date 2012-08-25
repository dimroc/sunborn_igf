#pragma once

#include "FXDecl.h"
#include "SunStates.h"


#define SKY_EFFECT_NAME                 L"Effects/Sky.fx"
#define SKY_TECHNIQUE_NAME              "SkyRender"

//! pushes the sky dome further out from min skydome radius based on input terrain.
#define SKYDOME_RADIUS_MULTIPLIER       2.f 

using std::vector;
using std::deque;

//! Stores and Renders the actual sky dome with a custom sphere to accomodate day/night cycle.
class CSky
{
private:
    typedef MESHVERT SkyVert;

    //
    //! Quad billboard holding the sun and lightshafts
    struct SBigStar {
        static LPDIRECT3DVERTEXBUFFER9 pVB;
        static D3DXVECTOR3 worldcenter;
        static float magnitude;

        LPDIRECT3DTEXTURE9 pTex;
        D3DXVECTOR3 start_pos, pos, lightPos, lightDir;
        D3DXMATRIX mWorld, mScale;
		float orbit_speed, angle, fLightTimeOffset, time_offset, angle_offset, alpha_multiplier;
        

        SBigStar(IDirect3DDevice9* pd3dDevice,
                LPCWSTR texFileName,    // texture for star
                float orbit_speed,  // speed star rotates around terrain. 1 is normal day/night speed.
                float angle,    // angle along south/north plane, so not directly overhead. (rotates from east to west)
                float time_offset,  // the rotation position offset from noon(straight above) in time.
                float xscale,
                float zscale); 
        ~SBigStar();

        VOID OnFrameMove(float fTimeofDay);
        VOID OnFrameRender(IDirect3DDevice9* pd3dDevice);

        static VOID SetWorldOrbit(const D3DXVECTOR3& center, float rad);
        static VOID InitQuad(IDirect3DDevice9* pd3dDevice);
    };

    //
    //! Used to keep time of day and control speed of time.
    struct SClock {     // let's base it on 24 hour clock for ease of calculations        
		static float FULL_DAY;
        float time, speed;

        SClock(float start_time, float clock_speed) : time(start_time), speed(clock_speed) {}
        ~SClock() {}
    };    

    //! Creates the triangle strip/fan of the sky dome with SkyVert being the vertex
    HRESULT CreateSphere(IDirect3DDevice9* pd3dDevice, int nRings, int nSegments);

private:

    LPDIRECT3DVERTEXBUFFER9         m_pSkyDomeVB;  //! VB holding the actual sky dome
    LPDIRECT3DINDEXBUFFER9          m_pSkyDomeIB;
    LPDIRECT3DTEXTURE9              m_pSkyTone; //! Sky tone to be used to interpolate colors from zenith to horizon.

    IDirect3DVertexDeclaration9*    m_SkyDecl; //! Vertex decl for sky rendering
    ID3DXEffect*                    m_SkyEffect; //! D3DX effect interface

    //! Cache handles of effect
    D3DXHANDLE                      m_SkyTechnique, 
                                    m_LightDirHandle, 
                                    m_WorldViewProjHandle, 
                                    m_InvTransWorldHandle,
                                    m_TimeOfDayHandle,

                                    m_SunColorHandle[2],
                                    m_SunVertexShaderPowerHandle[2],
                                    m_SunPixelShaderPowerHandle[2],
                                    m_SunVertexShaderMultiplierHandle[2];

    SClock                          m_Clock;
    bool                            m_bClockMoving;
    vector<SBigStar*>               m_vpBigStars;
    D3DXVECTOR4                     m_LightDir;
    D3DXVECTOR3                     m_LightRight, m_LightUp;

///////////////////////// SUN STUFF 

    /*! The Sun's color isn't really the color. It is the arithmetic operation you would like to
        do at the spot that makes up the son. So 120,0,0 adds 120 r at the sun location on sky dome.
        -255,-255,-255, 0 would make the sun black.

        The alpha component makes the sun location see through into the CLEARED buffer (which is white).
        */    
    /*  These variables affect the size and density of the sun. Lower is bigger except for the VSMult. */       


    SSunMorphState                  m_CurrentSunStates[2];
    SSunMorphState                  m_OldSunStates[2];
    //! if empty sun is constant at currentsunstate. Entries are akin to keyframed morphing states. 
    //! To set a chain of morphs, send in multiple states.
    deque<SSunMorphState>           m_dSunStates[2]; 

    float                           m_fMorphTime[2];
    bool                            m_bStartMorph[2];
    bool                            m_bUpdateSun[2];

    //
    //! TODO: Make moon(s?)
    
    UINT                            m_nVertices, m_nIndices, m_nSphereTriangles;    

    //! World matrix for skydome. Scale from radius 1 and translate to center.
    D3DXMATRIX                      m_World, m_InvTransWorld;
    D3DXVECTOR3                     m_vWorldCenter;
    float                           m_fSkyRadius;
public:
    static const D3DXVECTOR4 SKY_DEFAULT_SUN_COLOR;    

public:
    CSky(const D3DXVECTOR3* pvCenter, float fRadius);
    ~CSky(void);

    VOID OnCreateDevice(IDirect3DDevice9* pd3dDevice, 
                          LPCSTR skytoneFilename);

    VOID AddBigStar(IDirect3DDevice9* pd3dDevice,
                    LPCWSTR texFileName,    // texture for star
                    float orbit_speed,  // speed star rotates around terrain
                    float angle,    // angle along south/north plane. (rotates from east to west)
                    float time_offset, // the rotation position offset from noon(straight above) in time.
                    float xscale = 60.0f,
                    float zscale = 60.0f); 


    VOID SetWorldPosition(const D3DXVECTOR3& center, float radius);
    VOID GetWorldPosition(D3DXVECTOR3 *pOut, float *pRadius) {  //! Get this worlds position.
        pOut->x = m_vWorldCenter.x; pOut->y = m_vWorldCenter.y; pOut->z = m_vWorldCenter.z;
        *pRadius = m_fSkyRadius;
    }

    const D3DXVECTOR3* GetLightPos() { assert(m_vpBigStars.size() > 0); return &m_vpBigStars[0]->lightPos; }
    const D3DXVECTOR3* GetLightRight() { return &m_LightRight; }
    const D3DXVECTOR3* GetLightUp() { return &m_LightUp; }
    const D3DXVECTOR4* GetLightDir() { return &m_LightDir; }
    const D3DXVECTOR4* GetSunColor(int idx) { return &m_CurrentSunStates[idx].color; }    
    float GetTimeOfDay() { return m_Clock.time; }
    VOID SetTimeOfDay(float fClockTime) { m_Clock.time = fClockTime; }

    VOID SetLightDirByTimeOfDay(float fLightDirByTimeOfDay);
    
    VOID SetSunColor(const D3DXVECTOR4& vSunColor, int idx) 
    { 
        assert(idx >= 0 && idx < 2);
        m_CurrentSunStates[idx].color = vSunColor; m_bUpdateSun[idx] = true; 
    }

    //! Sun Setters for variables in shader. Lower value = bigger sun. To mess with sun in real-time.
    VOID SetSunVSPower(float vspower, int idx) { m_CurrentSunStates[idx].vspower = vspower; m_bUpdateSun[idx] = true; }
    VOID SetSunPSPower(float pspower, int idx) { m_CurrentSunStates[idx].pspower = pspower; m_bUpdateSun[idx] = true; }
    VOID SetSunVSMult(float vsmult, int idx) { m_CurrentSunStates[idx].vsmult = vsmult; m_bUpdateSun[idx] = true; }

    float GetSunVSPower(int idx) { return m_CurrentSunStates[idx].vspower; }
    float GetSunPSPower(int idx) { return m_CurrentSunStates[idx].pspower; }
    float GetSunVSMult(int idx) { return m_CurrentSunStates[idx].vsmult; }

    //! Addes a SunMorphState to the queue of morph events must go through. See "SunStates.h" to fill out your own! 
    VOID PushBackSunState(const SSunMorphState &sunState, int idx) { m_dSunStates[idx].push_back(sunState); }

    VOID ToggleClockMoving()  { m_bClockMoving = !m_bClockMoving; } //! starts/stops clock

    VOID OnFrameMove(float fElapsedTime, const D3DXMATRIX* pmViewProj);
    HRESULT OnFrameRender(IDirect3DDevice9 *pd3dDevice);    

    HRESULT OnResetDevice(IDirect3DDevice9 * pd3dDevice);    
    HRESULT OnLostDevice();
};
