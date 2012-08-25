#pragma once

#include "xmodelheirarchy.h"

class CModelInstance
{
private:
    CXModelHeirarchy*           m_pXmodel;  // Pointer to the actual Geometry of this instance. NOT OWNER.

    LPD3DXFRAME                 m_pFrameRoot;   // Frame Root of this Instance for heirarchies

    // Animation 
    ID3DXAnimationController*   m_pAnimController;  // Animation controller for this instance.
    UINT                        m_nCurrentAnimationSet;
    DWORD                       m_nCurrentTrack;
    float                       m_fCurrentTime;

    vector<float>               m_vfSpeedAdjust; // divides fElapsedTime in Model's elapsed time to slow down animation.    

    // toon stuff
    float                       m_fShades, m_fMinShade;
    float                       m_fThreshold;
    D3DXVECTOR3                 m_vEdgeColor;

    // handles
    D3DXHANDLE                  m_ShadesHandle, 
                                m_MinShadeHandle,
                                m_ThresholdHandle,
                                m_EdgeColorHandle,
                                m_ShadowMtxHandle,
                                m_ShadowMapHandle,
                                m_LightDirHandle,
                                m_CameraPosHandle,
                                m_ViewProjHandle,
                                m_WorldHandle;

private:
    //! This function copies the D3DXFRAME heirarchy passed in AND creates a copy of the original Heirarchy's *pMeshContainer.
    //! This function is super tightly coupled with XModelHeirarchy.
    LPD3DXFRAME CopyFrameHeirarchy( LPD3DXFRAME pOtherRoot, bool bCopyMeshContainer = true );
    VOID CopyFrameHelper( D3DXFRAME_DERIVED** ppReturn, D3DXFRAME_DERIVED* pOther, bool bCopyMeshContainer );

    VOID CleanUpCopyHeirarchy(LPD3DXFRAME pFrame, bool bCopyMeshContainer = true);

    //! Advances animation.
    void AdvanceAnimController( float fElapsedTime, LPD3DXANIMATIONCALLBACKHANDLER pCallBackHandler ) { 
        if( m_pAnimController )  
            m_pAnimController->AdvanceTime( fElapsedTime, pCallBackHandler ); 
    }
    UINT GetNumberOfAnimationSets() const { if(m_pAnimController) return m_pAnimController->GetNumAnimationSets(); return 0; }

public:
    CModelInstance( CXModelHeirarchy* pXmodelHeirarchy );    
    ~CModelInstance();

    VOID OnFrameMove( float fElapsedTime, const D3DXMATRIX* mWorld );

    //! Sets up pEffect with light and draws the single mesh represented in this object.
    void OnFrameRender( IDirect3DDevice9 *pd3dDevice, 
                        const D3DXMATRIX* pMatProj, const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld,
                        const D3DXVECTOR3* pEyePt, const D3DXVECTOR4* pLightDir,
                        LPDIRECT3DTEXTURE9 pTex);

    //! Sets the light's viewspace and the shadowmap texture to be used in OnFrameRender by the effect/shaders.
    void SetShadowMap( IDirect3DDevice9 *pd3dDevice, const D3DXMATRIX* pShadowMtx, LPDIRECT3DTEXTURE9 pShadowMap );

    //! Renders the z's to the back buffer into all channels (rgb) only if alpha > 0.f. 
    void RenderShadowMap( IDirect3DDevice9 *pd3dDevice, 
                        const D3DXMATRIX* pMatProj, const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld,
                        const D3DXVECTOR3* pLightPos, const D3DXVECTOR4* pLightDir );

    //! Plays an animation once and then stops at the last frame. RETURN: time period of animation
    float PlayAnimationSetOnce(UINT idx);   
    void SetAnimationSet(UINT idx);
	void SelectNextAnimationSet();
    double GetAnimationPeriod(UINT idx) { if(m_pXmodel->IsHeirarchy()) return m_pXmodel->GetAnimationPeriod(idx); return 0; }

    //! Used as multiplier against fElapsedTime to alter animation speed.
    void AdjustAnimationSpeed( UINT idx, float fSpeedAdjust ) 
    { 
        assert(m_pXmodel->IsHeirarchy()); 
        m_vfSpeedAdjust[idx] = fSpeedAdjust; 
    }

    float GetObjectRadius() { return m_pXmodel->m_fObjectRadius; }
    const D3DXVECTOR3* GetObjectExtremities() { return &m_pXmodel->m_vObjectExtremities; }
    const D3DXVECTOR3* GetObjectCenter() { return &m_pXmodel->m_vObjectCenter; }

    CXModelHeirarchy* GetXModel() { return m_pXmodel; }

    // ---------------------------- TOON SETTERS/GETTERS -------------------------------------
    VOID SetNumShades(int nShades) { m_fShades = (float)nShades; }
    VOID SetMinShade(int minShade) { 
        assert(minShade < (int)m_fShades);
        m_fMinShade = (float)minShade;
    }    
    VOID SetSilhouetteThresh(float fThresh) { m_fThreshold = fThresh; }
    VOID SetEdgeColor(const D3DXVECTOR3* vColor) { m_vEdgeColor = *vColor; }

	int GetNumShades() { return (int)m_fShades; }
	int GetMinShade() { return (int)m_fMinShade; }
	float GetSilhouetteThresh() { return m_fThreshold; }
	const D3DXVECTOR3* GetEdgeColor() { return &m_vEdgeColor; }
};
