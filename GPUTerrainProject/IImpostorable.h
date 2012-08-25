#pragma once

#include "globaldefine.h"
#include "FXDecl.h"

/*! TODO: If frames are a SERIOUS issue, here is the drastic option:
            Render all impostors to ONE RenderTarget(dynamic texture),
            stuff all vertices with appropriate RECTF into ONE VB,
            and call only ONE Draw Primitive. 

            Biggest Draw back:- Loss of quality in on impostor (will look blurrier)
                                Have to split up render target every frame and shit.
*/

#define                     IMPOSTOR_DELTA_AT_ANGLE_THRESH      .99995f  // 0.99995f
#define                     IMPOSTOR_DELTA_UP_ANGLE_THRESH      .9995f  // 0.995f

// TODO: Make larger as u get further away.
#define                     IMPOSTOR_DELTA_DISTANCE_THRESH      7.f //! distance before update (normally 8) 

#define                     IMPOSTOR_DISTANCE_FACTOR            2.5f // distance between rendering real clouds and impostors

/*!
Base Function for classes that intend to be Impostors when a certain distance from camera.
Not virtual for a reason. I don't want Polymorphism because I intend to batch render by
VB and shaders, thus needing the type information at higher levels.

Mean to aid implementation of children class that will be impostors, 
hence the protected constructor.
*/
class IImpostorable
{
protected:
    static const D3DFORMAT SurfaceFormat        =       D3DFMT_A8R8G8B8;    
    static ULONG s_OldStates[10]; //! To Save state before SetDevice and Clear Device afterwards.

    const int SurfaceWidth;
    const int SurfaceHeight;
    const D3DXVECTOR3 SurfaceCenterVector;

    LPD3DXRENDERTOSURFACE   m_pRenderToSurface;
    LPDIRECT3DTEXTURE9      m_pDynamicTexture;
    LPDIRECT3DSURFACE9      m_pTextureSurface;
    ID3DXEffect*            m_pShadowMapEffect;       // D3DX effect interface

    D3DXVECTOR3 m_vecPrevWorldUp, m_vecPrevLookAt, m_Center;
    float m_PrevDistance;
    bool m_bUseImpostor, m_bUpdateImpostor;

    BoxVertTex m_Vertices[4]; //! Vertices to store impostor!        

protected:
    //! Initializes the RenderToSurface for this object
    VOID InitRenderToSurface(IDirect3DDevice9 *d3dDevice);

    /*! Dots the 2 stored axii (worldUp, lookAt) against the new ones and returns difference. */
    void AngleCheck(const D3DXVECTOR3* pWorldUp, D3DXVECTOR3* pPrevWorldUp,
                    D3DXVECTOR3* pViewpointDif, D3DXVECTOR3* pPrevLookAt,
                    float &diffUp, float &diffAt); 

    //! Generates a 2D Billboard that encompasses the World Space bounding box sent in corners. Returns in worldSpaceVerts
    VOID GenerateWorldSpaceBBPlaneCoordinates(const D3DXVECTOR3* corners, 
                                              const D3DVIEWPORT9* pViewport, const D3DXMATRIX* pMatProj, 
                                              const D3DXMATRIX* pMatView, const D3DXMATRIX* pMatWorld);

    //! Return the Projection and View Matrices used to render to texture.
    VOID GetProjViewMatrices(D3DXMATRIX* pproj, D3DXMATRIX* pview, D3DXVECTOR3* pnewcenter,
                             const D3DXVECTOR3* pEyePt, const D3DXVECTOR3* pWorldUp);

protected:
    /*! Only inherited classes can be Impostorable!
        This creates the RenderTarget, the DynamicTexture, and the surface of that texture to rendertarget to.*/
    IImpostorable(int SurfaceWidth, int SurfaceHeight);    
    ~IImpostorable(void);

    HRESULT OnCreateDevice(IDirect3DDevice9* pd3dDevice);

    HRESULT OnLostDevice();
    HRESULT OnResetDevice(IDirect3DDevice9* pd3dDevice);

    //! PREREQ: CheckToUpdateImpostor, GenerateWorldSpaceBB and GetProjViewMat called. Renders the Impostor.
    HRESULT RenderImpostor(IDirect3DDevice9 *pd3dDevice);

    //! Checks whether to Render Impostor and if Impostor needs to be updated
    bool CheckToUpdateImpostor(bool &bShouldRenderImpostor,   //! returns whether object is in the distance to use impostor
                               const D3DXVECTOR3* pWorldUp,     //! camera's world up
                               D3DXVECTOR3* pViewpointDif,  //! vector from camera location to object location
                               float fObjectScale,    //! scale of object with which to calculate distance needed for impostor
                               bool bForce = false);    //! force impostor update.

    //! Prepares the projection and view matrices on the device for rendering object to impostor.
    VOID PrepareRenderToImpostor(LPDIRECT3DDEVICE9 pd3dDevice,  // duh
                                 const D3DXVECTOR3* m_corners,  //! corners of the bounding box as defined in CQuadTree::QTNode
                                 const D3DXVECTOR3* pEyePt, //! Position of Camera
                                 const D3DXVECTOR3* pObjPt, //! Position of object to be rendered to impostor
                                 const D3DXVECTOR3* pWorldUp, //! Camera's world up.
                                 const D3DVIEWPORT9* pViewport, //! Viewport of the d3dDevice.
                                 const D3DXMATRIX* pMatProj, //! Projection matrix of rendering
                                 const D3DXMATRIX* pMatWorld);  //! World matrix of object.

    //! Restores device from the function above. Basically just calls m_pRenderToTarget->EndScene().
    VOID RestoreRenderToImpostor();

    bool IsUsingImpostor() { return m_bUseImpostor; }
    const D3DXVECTOR3& GetCenterVector() { return m_Center; }

public:
    static HRESULT SetDevice(IDirect3DDevice9 *pd3dDevice);
    static HRESULT ClearDevice(IDirect3DDevice9 *pd3dDevice);

private:    // prevent usage of assignment operator for now. 
    IImpostorable& operator=(const IImpostorable &rhs); // disallow assignment operator
    IImpostorable(const IImpostorable& rhs); // disallow copy constructor
};
