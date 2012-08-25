#pragma once

#include "./iterrain.h"
#include "globaldefine.h"
#include "Pillar.h"

#ifndef S_OK
#define S_OK  ((long)0x00000000L)
#endif

//! Base for classes handling Occlusion Culling and is OWNER of a ITerrain * m_pTerrain
class ISpatialSubdivision
{
protected:
    

       

public:
    ISpatialSubdivision(ITerrain *pt)      
    {
        
    }
    virtual ~ISpatialSubdivision(void) { 
        
    }    

    //! Creates VBs IBs and meshes associated with the passed pd3dDevice.
    virtual UINT OnCreateDevice(IDirect3DDevice9 * pd3dDevice)     =   0;

    //! Draw the bounding boxes of the quads
    virtual UINT DrawBoundingBoxes()                                =   0;



    //! Calculations to be done before Render
    virtual VOID OnFrameMove( 
        const D3DXVECTOR3* pEyePt,
        const D3DXVECTOR3* pUp,
        const D3DXVECTOR3* pRight,
        const D3DXVECTOR3* pAhead,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView,        
        const D3DVIEWPORT9 * pViewport,
        const double fTime,
        const float fElapsedTime)                                =   0;    

    //! Rendering function
    virtual HRESULT OnFrameRender(
        const D3DXVECTOR3* pEyePt,  // Camera space stuff
        const D3DXVECTOR3* pUp,
        const D3DXVECTOR3* pRight,
        const D3DXVECTOR3* pAhead,
        const D3DXMATRIX* pMatProj,
        const D3DXMATRIX* pMatView,
        UINT param)                                              =   0;

    //! Used to create any D3DPOOL_DEFAULT Devices. Only Effect for now
    virtual HRESULT OnResetDevice(IDirect3DDevice9* pd3dDevice)  =   0;	

	//! Used to RECREATE any D3DPOOL_DEFAULT Devices Lost on minimize etc. Only Effect for now
	virtual HRESULT OnLostDevice()								=	0;

    //! Returns the center of this spatial subdivision.
    virtual D3DXVECTOR3 GetCenter()                             =   0;
    virtual float GetMinY()                                     =   0;
    virtual float GetMaxY()                                     =   0;

    //! Returns a bloom map if available (wow is this inappropriately placed).
    virtual LPDIRECT3DTEXTURE9 GetNotBloomMap()                    =   0;
    virtual gdef::COLOR GetBloomColor()                         =   0;

	virtual CPillar *GetPillarPointer(void) = 0;	// to be taken out along with reference to CPillar.h up top

    
};
