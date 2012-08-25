#pragma once
#include "Sky.h"
#include "sunstates.h"
#include "QuadTree.h"


#define                 SUNSTATE_INCREMENT              0.1f

//
// NOTE: There are only 256 cloud ImpostorAtlas slots available, don't make more than 256 clouds.
//

using std::vector;
using std::string;

class CCloudBatch;
class CPolySprite;
class CImpostorAtlas;


#define                 ONLYACTUALCLOUDS        0x01
#define                 ONLYIMPOSTORS           0x10


/*!
Renders the Sky with Atmospheric lighting and color value based on time of day (noon, dusk)
and renders clouds!
*/
class CCloudSky
{
private:
    typedef CQuadTree::QTNode QTNode;

    CSky* m_pSky;    

    vector<CCloudBatch*> m_vpCloudBatches; //!Clouds Batched by their Texture Atlas     
    vector<CCloudBatch*> m_vpCloudBatchesShadow; //!Clouds Batched by their Texture Atlas     

    CPolySprite *m_pPolySprite; //! Sprite that uses quads as opposed to point sprites.
    CImpostorAtlas *m_pImpostorAtlas;   //! Atlas for where clouds render their impostor.

    CPolySprite *m_pPolySpriteShadow; //! Sprite that uses quads as opposed to point sprites. For shadow clouds.
    CImpostorAtlas *m_pImpostorAtlasShadow;   //! Atlas for where clouds render their impostor for shadowmap lightspace.

    LPDIRECT3DTEXTURE9 m_pCloudAtlas; //! The Cloud Atlas from which the CClouds blocks are taken.

    string m_strCloudMap, m_strCloudAtlasTex, m_strSkyToneTex;
    int m_nCellSpacing, m_nHeightSpacing;

private:
    set<CCloud*> m_setActualClouds; //! Stores which clouds should be rendered with sprites while CloudImpostorAtlas holds the impostors.

public:
    CCloudSky(LPCSTR pszCloudMap, LPCSTR pszCloudAtlasTex, LPCSTR pszSkyToneTex, const D3DXVECTOR3 *pSkyCenter, float fSkyRadius);
    ~CCloudSky(void);

    //! Populates leaves in QuadTree with clouds.
    VOID OnCreateDevice(IDirect3DDevice9* pd3dDevice);     

    //! Populates nodes with appropriate clouds
    VOID PopulateLeafNodesWithClouds(QTNode* proot);

	UINT RenderSky(IDirect3DDevice9* pd3dDevice, bool bZTest = false);

    //! Splits the visible clouds between those to be rendered with sprites and those with impostors.
    VOID SplitClouds();

    // PREREQ: SplitClouds()
    //! Prepares the device for render by placing sprite and VB etc on device.
    UINT RenderClouds(IDirect3DDevice9* pd3dDevice, 
                      const D3DXVECTOR3* pEyePt, 
                      const D3DXVECTOR3* pUp,
                      const D3DXVECTOR3* pRight,
                      const D3DXVECTOR3* pAhead,
                      UINT flag = 0 );
    UINT RenderClouds(IDirect3DDevice9* pd3dDevice, 
                      CPolySprite* pPolySprite, 
                      const D3DXVECTOR3* pEyePt,
                      const D3DXVECTOR3* pAhead,
                      UINT flag = 0);

    // Renders the cloud impostors updated in UpdateShadowClouds.
    UINT RenderCloudsToShadowMap(IDirect3DDevice9* pd3dDevice,
                                 const D3DXVECTOR3* pEyePt,
                                 const D3DXVECTOR3* pEyeAhead,
                                 const D3DXMATRIX* pProj,
                                 const D3DXMATRIX* pView);

    // Updates the impostors of the shadow clouds based on the passed in sun's light frustum.
    UINT UpdateShadowClouds(IDirect3DDevice9* pd3dDevice,
                            const D3DXMATRIX* pProj,
                            const D3DXMATRIX* pView,
                            const D3DVIEWPORT9* pViewPort);

    //! Renders sky to texture for bubble light refraction after it updates position of the Sky condiments (sun stars and shit) 
    VOID OnFrameMove(IDirect3DDevice9* pd3dDevice, float fElapsedTime, const D3DXMATRIX* pmViewProj, const D3DXVECTOR3* pEyePt);

    /*! Saves clouds to disk: <nbatches <nclouds in first batch <clouds ...n>> <nclouds in second batch <clouds ...n>> .. nbatches>*/
    VOID SaveClouds(LPCSTR cloudFilename);  //! Saves Clouds to disk. DOES NOT COMPENSATE FOR CELL/HEIGHTSPACING
    VOID LoadClouds(LPCSTR cloudFilename);  //! Loads clouds from disk. Also doesn't care about cell/height spacing atm (so wtf are there paramters)

    //
    // SKY STUFF
    //
    // const float GetLightStrength() { return blah; }

    const D3DXVECTOR3* GetLightPos() { return m_pSky->GetLightPos(); }
    const D3DXVECTOR3* GetLightUp() { return m_pSky->GetLightUp(); }
    const D3DXVECTOR4* GetLightDir() { return m_pSky->GetLightDir(); }
    
    const D3DXVECTOR4* GetSunColor(int idx) { return m_pSky->GetSunColor(idx); }
    float GetTimeOfDay() { return m_pSky->GetTimeOfDay(); }
    VOID SetTimeOfDay(float fClockTime) { m_pSky->SetTimeOfDay(fClockTime); }
    VOID ToggleDayMovement() { m_pSky->ToggleClockMoving(); }

    VOID SetLightDirByTimeOfDay(float fLightDirByTimeOfDay) { m_pSky->SetLightDirByTimeOfDay(fLightDirByTimeOfDay); }

    //! HACK: to allow main file to demonstrate sun morph at key press.
    VOID PushBackSunState(const SSunMorphState& _sunState, int idx) { m_pSky->PushBackSunState(_sunState, idx); }

    VOID IncrementVSPower(int idx) { m_pSky->SetSunVSPower(m_pSky->GetSunVSPower(idx) + SUNSTATE_INCREMENT, idx); }
    VOID IncrementPSPower(int idx) { m_pSky->SetSunPSPower(m_pSky->GetSunPSPower(idx) + SUNSTATE_INCREMENT, idx); }
    VOID IncrementVSMult(int idx) { m_pSky->SetSunVSMult(m_pSky->GetSunVSMult(idx) + SUNSTATE_INCREMENT, idx); }

    VOID DecrementVSPower(int idx) { m_pSky->SetSunVSPower(m_pSky->GetSunVSPower(idx) - SUNSTATE_INCREMENT, idx); }
    VOID DecrementPSPower(int idx) { m_pSky->SetSunPSPower(m_pSky->GetSunPSPower(idx) - SUNSTATE_INCREMENT, idx); }
    VOID DecrementVSMult(int idx) { m_pSky->SetSunVSMult(m_pSky->GetSunVSMult(idx) - SUNSTATE_INCREMENT, idx); }

    //
    // End Sky Stuff
    //

    VOID ClearAtlasFlags(); // clears the impostor of atlas flags 

    CPolySprite* GetSprite() { return m_pPolySprite; }
    LPDIRECT3DTEXTURE9 GetCloudTex() { return m_pCloudAtlas; }
    const CCloudBatch* GetCloudBatch(int i) { return m_vpCloudBatches[i]; }
    size_t GetNumCloudBatches() { return m_vpCloudBatches.size(); }

    HRESULT OnLostDevice();
    HRESULT OnResetDevice(IDirect3DDevice9* pd3dDevice);
};
