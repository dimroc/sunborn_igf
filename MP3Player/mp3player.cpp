// mp3player.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "mp3player.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


Cmp3player::Cmp3player(void) : 
m_pGraph(NULL), m_pControl(NULL), m_pEvent(NULL), m_pSound(NULL),
m_bLoopSong(false), m_bInit(false), m_bLoopList(true),
m_nMP3Files(0)
{   
    // Initialize the COM library.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        m_bInit = false;
    m_bInit = true;
}

Cmp3player::~Cmp3player(void)
{
    CleanUp();
    CoUninitialize();
}

bool Cmp3player::Initialize()
{
    if(m_bInit == false)
        return m_bInit;

    HRESULT hr;
    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
        IID_IGraphBuilder, (void **)&m_pGraph);
    if (FAILED(hr))
    {     
        m_bInit = false;
        return false;
    }

    hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
    hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (void **)&m_pEvent);
    
    if (SUCCEEDED(hr))
        m_bInit = true;
    else
        m_bInit = false;
    return m_bInit;
}

UINT Cmp3player::AddFileToList(LPCWSTR fn)
{
    if(m_nMP3Files >= 100)
        return 0;
    UINT nReturn = m_nMP3Files++;
    StringCchCopyW(m_pszwMP3FileList[nReturn], 256, fn);    
    return nReturn;
}

HRESULT Cmp3player::PlayFromList(UINT nSongIdx, HWND hWnd, UINT RESOURCE_SYMBOL) 
{
    HRESULT hr = S_FALSE; 
    CleanUp();
    Initialize();
    
    if(m_bInit && nSongIdx < m_nMP3Files)
    {
        hr = m_pEvent->SetNotifyWindow((OAHWND)hWnd, RESOURCE_SYMBOL, 0);
        // Build the graph. 
        hr = m_pGraph->RenderFile(m_pszwMP3FileList[nSongIdx], NULL);
        if (SUCCEEDED(hr))
        {
            // Run the graph.
            m_nLastIdx = nSongIdx;
            hr = m_pControl->Run();
        }    
    }
    return hr;
}

VOID Cmp3player::HandleGraphEvent(HWND hWnd, UINT nRESOURCEID)
{
    if(!m_pEvent)
        return;

    long evCode;
    LONG_PTR param1, param2;   
    while(SUCCEEDED(m_pEvent->GetEvent(&evCode, &param1, &param2, 0))) {      
        m_pEvent->FreeEventParams(evCode, param1, param2);
        switch(evCode) {
        case EC_COMPLETE:   // loop bg music            
            m_pControl->Stop();
            if(m_bLoopSong)
            {   
                // CleanUp();                
                PlayFromList(m_nLastIdx, hWnd, nRESOURCEID);
                return;
            }
            else if(m_bLoopList)
            {   
                m_nLastIdx++;
                if(m_nLastIdx >= m_nMP3Files)
                    m_nLastIdx = 0;
                if(m_nLastIdx < m_nMP3Files)
                    PlayFromList(m_nLastIdx, hWnd, nRESOURCEID);
                return;
            }
            // else fall thru
        case EC_USERABORT: // Fall through.
        case EC_ERRORABORT:
            m_pControl->Stop();
            return;           
        }      
    }
}

VOID Cmp3player::CleanUp() {
    if(m_pControl) {
        m_pControl->Release();
        m_pControl = NULL;
    }
    if(m_pEvent) {        
        m_pEvent->Release();
        m_pEvent = NULL;
    }
    if(m_pGraph) {
        m_pGraph->Release();
        m_pGraph = NULL;
    }    
}
