#pragma once

#include "GlobalDefine.h"

using std::vector;
using std::sort;
using gdef::RECTF;

//! PREREQ: Can only batch render 2^16 / 6 Sprites (~11,000). Please break up otherwise.
class CPolySprite
{    
public:
    struct PolySpriteVertex { // 32 BYTES!!
        static const UINT FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 ;

        D3DXVECTOR3 pos;                
        ULONG color;
        float u,v;

        PolySpriteVertex() {}
        PolySpriteVertex(const D3DXVECTOR3 &vec, ULONG _color, float _u, float _v) :
            pos(vec), color(_color), u(_u), v(_v)
        {} 
    };
private:
    struct DrawInfo {
        LPDIRECT3DTEXTURE9 pTex;
        RECTF rectf; // Rect on tex to display
        D3DXMATRIX mtx;
        PolySpriteVertex verts[6]; // Used only if user sends in world space vertices.        
        D3DXVECTOR3 vCenter;
        bool bWorldSpace, bUsingVerts;
        UINT color;
        DrawInfo() {}
        DrawInfo(LPDIRECT3DTEXTURE9 _pTex, const RECTF &_rectf, const D3DXMATRIX &_mtx, UINT _color);          
    };

private:
    static const int NUMBER_OF_TODRAW_RESERVE_SPOTS         =       1000;
    static const int MAX_VB_SIZE                            =       sizeof(PolySpriteVertex) * 65535;

    LPDIRECT3DDEVICE9 m_pd3dDevice;    
    LPDIRECT3DVERTEXBUFFER9 m_pVB;  //! VB holding trilist to polysprites. OWNER

    vector<D3DXVECTOR3> m_vQuadCoordinates, m_vScreenAlignedCoordinates;
    
    D3DXMATRIX m_mtxIdentity;
    RECTF m_fullRectf;

    // normals defining camera space. Used for billboarding and depth sorting (back2front etc)
    D3DXVECTOR3 m_vecCameraAhead, m_vecCameraUp, m_vecCameraRight, m_vecCameraPos;
    D3DXVECTOR3 m_vecCalcWorldAhead, m_vecCalcWorldUp, m_vecCalcWorldRight;

    //! the World's up vector for World-Oriented Billboarding
    D3DXVECTOR3 m_vecDefaultWorldUp, m_vecDefaultWorldRight, m_vecDefaultWorldAhead; 

    UINT m_Flags;   
    bool m_bSetCalled;
    
    vector<DrawInfo> m_vToDraw; 
    vector<int> m_vNumTillTexSwap; // Number of sprites ( = draw primitives * 2) until texture swap.

    ULONG m_OldStates[10]; //! To Save states at begin and restore at end.

private:
    VOID PopulateVB();    //! Populates the Vertex Buffer for all entries in the ToDraw vector.    
    VOID DrawSprites(); //! Sets textures etc and calls the DrawPrimitive for rendering.

    //! PREREQ: Needs allocated 6 spots in vb for trilist. 
    //! Creates all the vertices in World Space in the Vertex Buffer.
    VOID CreateSprite(PolySpriteVertex *pvb, 
                        const D3DXMATRIX &mtx, 
                        const RECTF &rectf,                         
                        ULONG color);

    VOID CalcWorldOrientUpTransformation();
    VOID CalcWorldOrientRightTransformation();
    VOID CalcWorldOrientAheadTransformation();

    //
    // Billoboarding Techniques
    //
    VOID ScreenAlignQuad(PolySpriteVertex *pvb);
    VOID WorldOrientAlignQuad(PolySpriteVertex *pvb);
    VOID ViewPointAlignQuad(PolySpriteVertex *pvb, const D3DXVECTOR3 &center);

    /////////////////////// FUNCTORS to sort DrawInfo entries ////////////////////////////

    struct TextureSortFunctor {
        bool operator() (const DrawInfo &di1, const DrawInfo &di2) const;
    };

    struct DepthSortFunctor {
        bool m_bBackToFront;
        D3DXVECTOR3 m_lookAt, m_eyePt;

        DepthSortFunctor(const D3DXVECTOR3 &lookAtNormal, const D3DXVECTOR3 & eyePt, bool bBackToFront);
        bool operator() (const DrawInfo &di1, const DrawInfo &di2) const;
    };


public:
    // FLAGS
    static const int SCREENALIGN_BILLBOARD            =       (0x01 << 0);
    static const int VIEWPOINTALIGN_BILLBOARD         =       (0x01 << 1);    
    
    static const int BACKTOFRONT                      =       (0x01 << 10);
    static const int FRONTTOBACK                      =       (0x01 << 11);
    static const int SORTTEXTURE                      =       (0x01 << 12);

    //! These use the World's default vector instead of camera's. Don't use these unless you know what you're doing
    static const int WORLDORIENTAHEAD                 =       (0x01 << 5); 
    static const int WORLDORIENTUP                    =       (0x01 << 6);    
    static const int WORLDORIENTRIGHT                 =       (0x01 << 7);

private:
    static const int WORLDORIENT                      = WORLDORIENTAHEAD | WORLDORIENTUP | WORLDORIENTRIGHT;

public:
    CPolySprite(LPDIRECT3DDEVICE9 pd3dDevice);
    ~CPolySprite(void);

    VOID Begin(UINT flags = SORTTEXTURE); //! Sets renderstates for the batch drawing of textures.

    //! Add entry to draw batch. If pRectf == NULL, uses whole tex. Must be called inside BeginDrawBatch-EndDrawBatch.
    VOID Draw(              
            LPDIRECT3DTEXTURE9 pTex, 
            const RECTF *pRectf /*rect on tex*/, 
            const D3DXMATRIX *pMtx,
            bool bWorldSpace = false,
            UINT color = 0xffffffff); 

    //! Add entry to draw batch.
    VOID Draw(              
            LPDIRECT3DTEXTURE9 pTex, 
            PolySpriteVertex verts[6],
            const D3DXVECTOR3 *pCenter,
            bool bWorldSpace = false); 

    VOID Flush(); //! Sends current batch to video card.

    VOID End(); //! restors render states, etc.

    //! PREREQ: Normalized look at vector
    //! Used to billboard and sort textures (back to front etc).
    VOID SetCameraSpace(const D3DXVECTOR3 *pCameraPos,
                        const D3DXVECTOR3 *pCameraUp,
                        const D3DXVECTOR3 *pCameraRight,
                        const D3DXVECTOR3 *pCameraAhead);

    VOID OnLostDevice();
    VOID OnResetDevice();
};
