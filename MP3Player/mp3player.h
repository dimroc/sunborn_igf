#pragma once

#ifdef MP3PLAYER_EXPORTS
#define MP3PLAYER_API __declspec(dllexport)
#else
#define MP3PLAYER_API __declspec(dllimport)
#endif

#include <dshow.h>
#include <dsound.h>

class MP3PLAYER_API Cmp3player {

private:
    IGraphBuilder *m_pGraph;
    IDirectSoundBuffer8 *m_pSound;
    IMediaControl *m_pControl;
    IMediaEventEx *m_pEvent;

    bool m_bInit, m_bLoopSong, m_bLoopList;    
    WCHAR m_pszwMP3File[256];
    WCHAR m_pszwMP3FileList[100][256];
    UINT m_nMP3Files, m_nLastIdx;

private:
    bool Initialize();

public:
    Cmp3player(void);
    ~Cmp3player(void);

    UINT AddFileToList(LPCWSTR); // returns song index
    HRESULT PlayFromList(UINT nSongIdx, HWND, UINT);
    // HRESULT Stop(LPCWSTR, HWND, UINT);

    VOID LoopSong(bool _bLoop) { m_bLoopSong = _bLoop; }
    VOID LoopList(bool _bLoopList) { m_bLoopList = _bLoopList; }

    BOOL IsEventInit() { return m_pEvent != NULL; }
    VOID HandleGraphEvent(HWND hWnd, UINT nRESOURCEID);

    VOID CleanUp();
};
