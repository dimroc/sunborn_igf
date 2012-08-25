#pragma once
#include "globaldefine.h"

using gdef::RECTF;
using std::vector;
using std::ostream;
using std::istream;

struct SCloudInfo;
class CCloud;
class CImpostorAtlas;
class CPolySprite;

#define         SUBCLOUD_DISTANCE_THRESH        1.5f

//! TODO: Need to create a more powerful cloud changer
//! TODO: Change these to member variables, make setters/getters, and have an interp function between settings.
#define         CLOUD_DAY_SHADE                 255 // Color of clouds at the sun peak
#define         CLOUD_NIGHT_DIF                 0 // Shade to subtract from CLOUDDAYSHADE
//! OFF FOR NOW

#define         COLOR_TIME_UPDATE               .2f // update color/shading every x seconds

///////////////////////////////////////////////////////////////////////////////////////////
/*!
Renders ubr clouds using a cluster of sprites that are camera oriented!
If greater than a certain distance from camera, it renders on a texture(impostor)
and then renders that!
*/
class CCloudBatch
{
private:
    bool bOwnSprite;    
    vector<CCloud*> m_vpClouds; //! vector of clouds that make up the entire batch of clouds.

    CPolySprite* m_pCloudSprite; //! Sprite that holds the texture atlas for the clouds, a 512x512. NOT OWNER
    CImpostorAtlas* m_pImpostorAtlas; // not owner dont do shit. This is where clouds render their impostor. NOT OWNER

    UINT m_CloudBatchID; // Unique ID that marks the CClouds created with which batch it belongs to. Helpful for rendering.

    static UINT m_UniqueCloudBatchID; //! Passed to CCloud to identify which Batch it belongs to
public:    
    CCloudBatch(CPolySprite *pSprite, CImpostorAtlas *pImpostorAtlas); //! Uses the passed sprite, DOES NOT DEAL WITH SPRITE MEMORY (RELEASE,etc).
    ~CCloudBatch(void);

    CCloud* AddCloud(const SCloudInfo &ci); //! Constructs and adds a cloud based on SCloudInfo which gives position, dimensions,etc.        
    const vector<CCloud*> &GetClouds() const { return m_vpClouds; }
};

///////////////////////////////////////////////////////////////////////////////////////////
/*! Holds IDs to different cloud types and the corresponding position of a texture block in the Sprite */
struct SCloudInfo
{
    static const UINT CT_NIMBOSTRATUS           =   1;  //dark low level clouds bringing rain
    static const UINT CT_STRATOCUMULUS          =   2;  //low, lumpy layer of clouds that is sometimes accompanied by weak intensity precipitation
    static const UINT CT_FAIRCUMULUS            =   3;  // Fair weather cumulus. floating cotton
    static const UINT CT_CUMULUSNIMBUS          =   4;  //more vertically developed than fair weather cumulus. They can exist as individual towers
    static const UINT CT_ALTOCUMULUS            =   5;  // field of parallel cotton balls    
    static const UINT CT_CIRRUS                 =   6;  // super high level clouds, thin and wispy

    static const UINT CTARR_NIMBOSTRATUS[6];
    static const UINT CTARR_STRATOCUMULUS[6];
    static const UINT CTARR_FAIRCUMULUS[6];
    static const UINT CTARR_CUMULUSNIMBUS[6];
    static const UINT CTARR_ALTOCUMULUS[6];
    static const UINT CTARR_CIRRUS[6];

    static const UINT* PCTARR_TYPES[6];

    static const int CLOUD_BLOCKS_PER_DIMENSION     =   4;  //! Means there are 4x4 texture patches in cloud atlas.

    static RECTF GetPuffTexRect(int pos); //! Returns the srcRect in the texture that holds the pos' texture 0-15 row first.

    D3DXVECTOR3 pos;
    UINT CloudType;
    int nPuffs;
    float puffScale;
    float heighthalf, widthhalf, depthhalf; // y x z

    SCloudInfo();
    SCloudInfo(UINT cloudType, D3DXVECTOR3 pos, int nPuffs, float puffScale, float widthhalf, float heighthalf, float depthhalf);
    ~SCloudInfo();
};

///////////////////////////////////////////////////////////////////////////////////////////
/*! Holds a single clouds information and each of the sprites info that make up the cloud */
class CCloud
{
private:
    struct SSubCloud {        
        static const D3DXVECTOR3 VECSPRITECENTER;
        
        RECTF srcRectf;   // source rect on cloud texture
        D3DXMATRIX mWorld;  // SRT transformation for sprite in the world
        D3DXVECTOR3 pos; // this is position relative to cloud SCloudInfo position in world
        float scale;    // for SRT
        float rotation; //! rotation of sprite on camera's near plane. so parallel.        
        float vertInterp; //! a value between 0-1 inclusive that represents the subclouds position from top to bottom. used for lighting to darken bottom.
        UINT color;

        SSubCloud(const RECTF &srcRectf, const D3DXVECTOR3 &pos, float scale, float rotation, float vertInterp);
        ~SSubCloud() {}
    };

    vector<SSubCloud> m_vSubClouds; //! The subclouds, individual sprites, make up the cloud, a cluster of sprites.
    SCloudInfo m_CloudInfo; 
    UINT m_BatchID; //! Which CCloudBatch this cloud belongs to.

    // Corner Vectors. 
    //  4 --- 5                    3 --- 2 - top
    //  |     |                    |     |
    //  0 --- 1  - bottom face     7 --- 6  
    D3DXVECTOR3 m_corners[8]; //! store AABB

    CPolySprite* m_pSprite; //! NOT OWNER, don't do shit.
    CImpostorAtlas* m_pImpostorAtlas; //! Not owner, don't do shit.

    float m_fTimeSinceLastColorUpdate, m_fColorTimeThresh;
    float m_fLargestDim; //! Largest dimension of AABB. Used to determine furthest distance before using Impostor.

    bool m_bVisible, m_bUpdated, m_bCheckedForDraw, m_bChangedColor, m_bUsingImpostor;
    int m_nAtlasId;
private:
    //! Updates color based on time of day and sunColor.
    VOID UpdateLighting(const D3DXVECTOR3* pSunPos, const D3DXVECTOR3* pLightDir,        
                            float fTimeOfDay, const D3DXVECTOR4* pSunColor);

public:
    //! Constructs the cloud by generating sprites with textures pertaining to the CloudType.
    //! Needs pSprite to render to impostort. Is NOT owner of the sprite.
    CCloud(CPolySprite *pSprite, CImpostorAtlas* pImpostorAtlas, const SCloudInfo &ci, UINT batchID); 
    ~CCloud(void);    

    VOID GenerateSubClouds();  //! Generates the subclouds making up the clouds based on the SCloudInfo paramaters.

    //! Updates Clouds state (lighting, pos etc.)
    VOID OnFrameMove(const D3DXVECTOR3* pSunPos, const D3DXVECTOR3* pLightDir, 
                         float fTimeOfDay, const D3DXVECTOR4* pSunColor, float fElapsedTime); 

    //! Updates Impostor if needed. Returns true if updated was needed.
    bool UpdateImpostor(IDirect3DDevice9 *pd3dDevice, const D3DXMATRIX* pMatProj, const D3DXMATRIX* pMatView, 
                         const D3DXMATRIX* pMatWorld, const D3DVIEWPORT9* pViewport, 
                         const D3DXVECTOR3* pEyePt, const D3DXVECTOR3* pWorldUp, const D3DXVECTOR3* pRight, const D3DXVECTOR3* pAhead,                          
                         LPDIRECT3DTEXTURE9 pTex);

    HRESULT RenderThruSprite(CPolySprite* pSprite, LPDIRECT3DTEXTURE9 pTex); //! PREREQ: Sprite has called Begin(flags) before and calls End() later.

    UINT GetBatchID() { return m_BatchID; }    
    const SCloudInfo &GetCloudInfo() const { return m_CloudInfo; }
    const D3DXVECTOR3* GetCorners() const { return m_corners; }

    bool IsVisible() { return m_bVisible; } //! returns whether it has been deemed visible by the ViewFrustumFull
    bool IsUpdated() { return m_bUpdated; }
    bool IsCheckedForDraw() { return m_bCheckedForDraw; }
    bool IsUsingImpostor() { return m_bUsingImpostor; }

    int GetAtlasId() { return m_nAtlasId; }

    VOID SetUpdated(bool b) { m_bUpdated = b; }
    VOID SetVisible(bool b) { m_bVisible = b; }
    VOID SetCheckedForDraw(bool b) { m_bCheckedForDraw = b; }
};

ostream& operator<< (ostream &os, const SCloudInfo& ci); //! Outputs cloud to ostream. Only SCloudInfo, does not output SubClouds.
istream& operator>> (istream &is, SCloudInfo& ci); //! Inputs SCloudInfo into cloud from IS.