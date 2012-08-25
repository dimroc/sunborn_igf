#pragma once

#include "GlobalDefine.h"

using std::vector;
using std::list;
using stdext::hash_map;
using gdef::RECTF;

class CSpriteAnimMgr
{
private:
    struct SAnimStill { // Holds the unique running anim pair and anim idx
        int idxRunning, idxAnim, idxStill;                
        RECTF frect;
        float fElapsedTime;
    };
    struct SAnimation { // holds the still numbers ( row = n/maxRows; col = n%maxCols) 
        int texIdx;
        CSound *pSound;
        vector<int> vStills;
        float speed;
    };
    struct STexture {
        LPDIRECT3DTEXTURE9 pTex;
        int nWidth, nHeight;
        int nRows, nCols;    
        float fHorizInterv, fVertInterv;
    };

    vector<STexture> m_vpTexs;
    vector<SAnimation> m_vAnimations;
    list<SAnimStill> m_lRunningAnimIdx;   // The Currently active animations

    list<SAnimStill>::iterator m_runningIt; // running iterator;

    typedef hash_map<int, list<SAnimStill>::iterator>  RunningEntriesHashMap;
    RunningEntriesHashMap m_hmRunningIterators; //! Stores all the running iterators and indices.

    int m_RunningID;

    LPDIRECT3DDEVICE9 m_pd3dDevice;

private:
    VOID UpdateRect(const STexture &texWrap, int nStill, RECTF &frect);

public:
    CSpriteAnimMgr(LPDIRECT3DDEVICE9 pd3dDevice);
    ~CSpriteAnimMgr(void);

    //! Updates running animations
    VOID OnFrameMove(float fElapsedTime);

    //! Adds a texture to be managed by the Animation Manager. 
    //! With the number of stills in each row and column passed.
    //! RETURNS: texture index
    int AddTexture(LPCWSTR texFileName, int nRows = 1, int nCols = 1);
    int AddTexture(LPCSTR texFileName, int nRows = 1, int nCols = 1);

    //! Adds an animation sequence that refers to an added texture
    //! RETURNS: the animations index
    int AddAnimation(int idxTex, const vector<int> &vStillIndices, float speed = 0.f, CSound *pSound = NULL);

    //! Adds animation to the running animations list and returns its runningAnimIndex.
    //! RETURNS: The runningAnimations index
    int StartAnim(int idxAnim);

    //! Removes a RUNNING animation from the running animations list. 
    //! Can have the same animation running more than once.
    VOID StopAnim(int runningAnimIdx);

    //! Returns Info to display current "Still" of animation given at runningAnimIdx.
    VOID GetAnimStill(int runningAnimIdx, LPDIRECT3DTEXTURE9 *ppTex, RECTF &frect);

    //! Pops off the next animation from anim manager. qucker than searching for specific index.
    //! Returns false when there are more animations. Is reset on OnFrameMove().
    bool GetNextAnimStill(LPDIRECT3DTEXTURE9 *ppTex, RECTF &frect);

    VOID ResetNextAnimStill()               { m_runningIt = m_lRunningAnimIdx.begin(); }
    
};
