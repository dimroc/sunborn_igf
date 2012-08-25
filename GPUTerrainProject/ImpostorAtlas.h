#pragma once

#include "iimpostorable.h"
#include "FXDecl.h"
#include "globaldefine.h"

using std::vector;
using gdef::RECTF;

class CPolySprite;

//! PURPOSE: Encapsulates the Rendering of Multiple Impostors to one texture
//!          minimizing the need to render a group of impostors to 
//!          one texture swap and one DrawPrimitive.
//! Each entry has 2 positions in the Atlas so as to have double buffering.
//! This allows us clear the swap.
class CImpostorAtlas :
                public IImpostorable   // implemented as impostor
{
private:
    struct SAtlasEntry {
        int swapid;
        bool bValid;    //! Whether this entry is still valid or can be invalidated and swapped.
        bool bToDraw;   //! Whether this entry should be drawn in RenderAtlas.
        RECT rect;  //! The rect on the ImpostorAtlas texture that holds this entry's impostor.
        RECTF rectf;    //! Same as above but in normalized floating point version.
        BoxVertTex verts[6]; // world space vertices of this impostor entry.
        D3DXVECTOR3 vCenter;
    };

    struct SAtlasEntryUpdate {  //! to store previous angle / threshold variables        
        D3DXVECTOR3 vecPrevWorldUp, vecPrevLookAt, vecCenter;
        float prevDistance;
        bool bUseImpostor, bUpdateImpostor;
    };

    struct PtrEntrySort 
    {   //! Sorts SAtlasEntries according to position from camera and it's lookAhead vector.
        const D3DXVECTOR3 *m_pEyePt, *m_pAhead;
        bool bBackToFront;
        PtrEntrySort(const D3DXVECTOR3 *_pEyePt, const D3DXVECTOR3 *_pAhead, bool _bBackToFront) :
            m_pEyePt(_pEyePt), m_pAhead(_pAhead), bBackToFront(_bBackToFront) {}
        bool operator() (const SAtlasEntry *pLeft, const SAtlasEntry *pRight) const
        {
            D3DXVECTOR3 vDifL = pLeft->vCenter - *m_pEyePt;
            D3DXVECTOR3 vDifR = pRight->vCenter - *m_pEyePt; 

            float fDifL = D3DXVec3Dot(&vDifL, m_pAhead);
            float fDifR = D3DXVec3Dot(&vDifR, m_pAhead);

            if(bBackToFront)
                return fDifL > fDifR;
            else
                return fDifL < fDifR;
        }
    }; //funcptrSort(pEyePt, pAhead);

private:
    const int m_nAtlasWidth, m_nAtlasHeight;
    const int m_nImpostorWidth, m_nImpostorHeight;
    const int m_nMaxImpostors;

    int m_nEntries;
    int m_nFlaggedToDraw;

    float m_fPixelOffsets[12]; //! Since DirectX renders to the middle of the pixel, but tex coordinates are top left, we offset this.

    vector<SAtlasEntry> m_vEntries;    //! TRILIST of Impostor vertices that have appropriate texture coordinates.
    vector<SAtlasEntryUpdate> m_vUpdates;   //! Holds the previous entry's update values to be checked if the current frame requires a new update.
    vector<RECT> m_vInvalidRectsToClear;    //! Holds the ids of the entries to be CLEARed. The ids correspond to the position in the atlas tex.

    D3DVIEWPORT9 m_ViewPort;    //! Viewport that holds most of the constants for RenderToAtlas;    

private:
    //! Invalidates current entry and places it on clear vector then returns its swaps entry.
    int SwapEntry(int id);  
    int ClampId(int id) { assert(id >= 0); if(id >= m_nMaxImpostors) return id - m_nMaxImpostors; return id; }

    VOID AssignTriListUVs(BoxVertTex* verts, const RECTF* rectf);

    VOID SetPixelOffsets(float *fPixelOffsets);

public:
    //! The Atlas dimensions define the giant texture used, the Impostor dimensions define 
    //! how much each render to atlas uses.
    CImpostorAtlas(int nAtlasWidth, int nAtlasHeight, int nImpostorWidth, int nImpostorHeight);
    ~CImpostorAtlas(void);

    HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice) { return IImpostorable::OnCreateDevice(pd3dDevice); }

    HRESULT OnLostDevice();
    HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pd3dDevice);

    //! Checks whether to Render Impostor and if Impostor needs to be updated
    bool CheckToUpdateImpostor(bool &bShouldRenderImpostor, 
                               const D3DXVECTOR3* pWorldUp, 
                               D3DXVECTOR3* pViewpointDif, 
                               float fSizeScale, 
                               int &id,
                               bool bForce = false);

    //! Renders an Impostor to the Atlas
    VOID StartRenderToAtlas(LPDIRECT3DDEVICE9 pd3dDevice,  // duh
                                 const D3DXVECTOR3* m_corners,  //! corners of the bounding box as defined in CQuadTree::QTNode
                                 const D3DXVECTOR3* pEyePt, //! Position of Camera
                                 const D3DXVECTOR3* pObjPt, //! Position of object to be rendered to impostor
                                 const D3DXVECTOR3* pWorldUp, //! Camera's world up.
                                 const D3DVIEWPORT9* pViewport, //! Viewport of the d3dDevice.
                                 const D3DXMATRIX* pMatProj, //! Projection matrix of rendering
                                 const D3DXMATRIX* pMatWorld,
                                 int oldId); 

    VOID StopRenderToAtlas();

    //! Render all valid impostors that have been flagged bToDraw.
    VOID RenderAtlas(LPDIRECT3DDEVICE9 pd3dDevice, 
                     const D3DXVECTOR3 *pEyePt = NULL, 
                     const D3DXVECTOR3 *pAhead = NULL, 
                     bool bSortBackToFront = false);

    VOID RenderAtlasToSprite(CPolySprite *pPolySprite); //! Renders all valid impostors through the passed in CPolySprite

    //! Render all valid impostors in greyscale with each value being the z depth on current rendertarget 0.
    VOID RenderAtlasForShadowMap(LPDIRECT3DDEVICE9 pd3dDevice,
                                 const D3DXVECTOR3* pEyePt,
                                 const D3DXVECTOR3* pEyeAhead,
                                 const D3DXMATRIX* pProj,
                                 const D3DXMATRIX* pView,
                                 const D3DXMATRIX* pWorld);

    //! Calls pd3d->CLEAR to clear all invalid RECTs. Call in the owner's OnFrameMove
    VOID ClearInvalidEntries(LPDIRECT3DDEVICE9 pd3dDevice);    

    //! Flag entry to draw
    VOID FlagToDraw(int id) { 
        if( id >= 0) {
            assert(id < (int)m_vEntries.size()); 
            assert(m_vEntries[id].bValid); 
            m_vEntries[id].bToDraw = true;
            m_nFlaggedToDraw++;
        }    
    }

    VOID ClearFlags() 
    {
        for(size_t i=0; i<m_vEntries.size(); i++)
            m_vEntries[i].bToDraw = false;  // it's been drawn
        m_nFlaggedToDraw = 0;
    }

    int GetNumMaxImpostors() { return m_nMaxImpostors; }
    int GetNumFlaggedToDraw() { return m_nFlaggedToDraw; }
    bool IsUsingImpostor(int id) { return m_vUpdates[ClampId(id)].bUseImpostor; }
};
