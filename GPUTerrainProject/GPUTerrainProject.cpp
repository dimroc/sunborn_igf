/*! --------------------------------------------------------------------------------------
// File: GPUTerrainProject.cpp
//
// Derived from EmptyProject:
// Empty starting point for new Direct3D applications
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------*/

#include "dxstdafx.h"
#include "resource.h"
#include "globaldefine.h"
#include "gamedefine.h"


#include "cpu_info/cpu_info.h" // Used to dump System information to Log
#include "gameroot.h"

using namespace std; // stl and iomanip
using namespace gdef;

#define                    LEVEL_TO_LOAD            "first_test.lvl"
#define                    MODEL_SET_TO_LOAD        "first_test.lml"
#define                    CHAR_SET_TO_LOAD         "first_test.lcl"
#define                    SPRITE_SET_TO_LOAD       "first_test.lal"
#define                    MASTER_SERVERS_TO_LOAD   "MasterServerInfo.txt"

//! Creates Logger instance which is later initialized in InitApp()
LOG_CREATE_DEFAULT
// NB: The logger never has to be initialized again, even if in other files and classes, just recreated.

// --------------------------------------------------------------------------------------
//! Global Variables
//--------------------------------------------------------------------------------------*/

CDXUTDialogResourceManager g_DialogResourceManager; //! manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          //! Device settings dialog
CDXUTDialog             g_HUD;                   //! dialog for standard controls
CDXUTDialog             g_UI;              //! dialog for sample specific controls

CGameRoot*              g_pGameRoot;        //! da game!

// --------------------------------------------------------------------------------------
//! UI and Other IDs, definitions
//---------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4

#define IDC_LOADHEIGHTMAP       11
#define IDC_FILEHEIGHTMAP       12

//--------------------------------------------------------------------------------------
//! Forward Declarations
//--------------------------------------------------------------------------------------*/
void            InitApp();   //! Initializes Terrain, SpatialSubdivision, Camera, and GUI elements
void            CleanupApp();   //! Cleans up memory allocated in InitApp()
void            RenderText(); //! Renders text such as FPS
void            DumpSystemToLog(); //! SELF DOCUMENTED CODE OWNAGE
void            DumpCapsToLog(); //! SELF DOCUMENTED CODE OWNAGE
UINT            LoadNewScene(LPCWSTR wszInput, bool bFirstRun = false); //! Loads a new terrain into SpatialS
void			BuildMenu(); //! Creates the menu and passes it to CGameRoot



bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK MouseProc(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext);
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );


// -------------------------------------------------------------------------------------- 
//! Initialize everything and go into a render loop

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackDeviceCreated( OnCreateDevice );
    DXUTSetCallbackDeviceReset( OnResetDevice );
    DXUTSetCallbackDeviceLost( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackMouse(MouseProc);
    DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );
        
    InitApp();

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( false, false, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"SunBorn" );
    DXUTCreateDevice( D3DADAPTER_DEFAULT, WINDOWED, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, IsDeviceAcceptable, ModifyDeviceSettings );

    DumpSystemToLog();
    DumpCapsToLog();

    // Start the render loop
    DXUTMainLoop();

    CleanupApp();   // Clean up App in here

#if defined(DEBUG) || defined(_DEBUG)
    // Functions to pinpoint memory leaks
    //_CrtDumpMemoryLeaks ();
    _CrtCheckMemory();
#endif

    return DXUTGetExitCode();
}

// -------------------------------------------------------------------------------------- 
//! Application Level Initialization, such as GUI and Heightmap

void InitApp() {   
    dprintf("rawr\n");    

    srand(UINT(time(NULL)));

    g_pGameRoot = new CGameRoot();

    g_pGameRoot->AssignWorldFile(LEVEL_TO_LOAD);
    g_pGameRoot->AssignModelSetFile(MODEL_SET_TO_LOAD);
    g_pGameRoot->AssignCharacterSetFile(CHAR_SET_TO_LOAD);
    g_pGameRoot->AssignSpriteAnimationSetFile(SPRITE_SET_TO_LOAD);

    g_pGameRoot->LoadMasterServersFile(MASTER_SERVERS_TO_LOAD);

    // Initialize dialogs
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_UI.Init( &g_DialogResourceManager );

    // Set up HUD
    g_HUD.SetCallback( OnGUIEvent ); int iY = 0; 
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 0, iY, 125, 22 );    
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 0, iY += 24, 125, 22, VK_F2 );

    // Set up UI
    g_UI.SetCallback( OnGUIEvent ); iY = 0;
    CDXUTEditBox *pEditBox;
    g_UI.AddEditBox( IDC_FILEHEIGHTMAP, L"rawr", 5, iY, 210, 20, false, &pEditBox );
    if( pEditBox )
    {
        pEditBox->SetBorderWidth( 1 );
        pEditBox->SetSpacing( 2 );        
    }
    g_UI.AddButton( IDC_LOADHEIGHTMAP, L"Load HeightMap", 0, iY += 24, 100, 22 );    
}


// --------------------------------------------------------------------------------------
//! Rejects any devices that aren't acceptable by returning false

bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Typically want to skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    if( pCaps->PixelShaderVersion < D3DPS_VERSION(2,0) ||
        pCaps->VertexShaderVersion < D3DVS_VERSION(2,0) )
	    return false;   // Fail if device doesn't support shader model 2.0+
    if( BackBufferFormat == D3DFMT_X1R5G5B5 || BackBufferFormat == D3DFMT_R5G6B5 )
        return false;    

    // 
    // Check Texture Filtering Capabilities
    //
    if(pCaps->TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR &&
       pCaps->TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR &&
       pCaps->TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR)
    {
        return true;
    }
    else
        return false;
}


//--------------------------------------------------------------------------------------
//! Before a device is created, modify the device settings as needed

bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
        pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif

    //
    // TODO: Turn on AntiAliasing
    //
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    DWORD quality1 = 0, quality2 = 0;
    if( SUCCEEDED(pD3D->CheckDeviceMultiSampleType( pCaps->AdapterOrdinal, 
                    pCaps->DeviceType, pDeviceSettings->pp.BackBufferFormat, 
                                WINDOWED, D3DMULTISAMPLE_3_SAMPLES, &quality1 ) ) &&
         SUCCEEDED(pD3D->CheckDeviceMultiSampleType( pCaps->AdapterOrdinal, 
                                pCaps->DeviceType, pDeviceSettings->pp.AutoDepthStencilFormat, 
                                WINDOWED, D3DMULTISAMPLE_3_SAMPLES, &quality2 ) ) )
    {
        pDeviceSettings->pp.MultiSampleQuality = quality2;
        pDeviceSettings->pp.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES;
        dprintf("***** ANTIALIASING ON I THINK!!\n");
    }
    dprintf("******* D3DMULTISAMPLE_QUALITY returned: quality1 = %d | quality2 = %d\n", quality1, quality2);

    return true;
}


//--------------------------------------------------------------------------------------
//! Create any D3DPOOL_MANAGED resources here 

HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );    

    V_RETURN( g_pGameRoot->OnCreateDevice(pd3dDevice) );    
    return S_OK;    
}


//--------------------------------------------------------------------------------------
//! Create any D3DPOOL_DEFAULT resources here 

HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnResetDevice() );
    V_RETURN( g_SettingsDlg.OnResetDevice() );
    
    if( g_pGameRoot )
        V_RETURN( g_pGameRoot->OnResetDevice(pd3dDevice) );  

    g_HUD.SetLocation( 0, 40 );
    g_HUD.SetSize( 170, 170 );
    g_UI.SetLocation( pBackBufferSurfaceDesc->Width-225, 5 );
    g_UI.SetSize( 220, 470 );

    return S_OK;
}


// --------------------------------------------------------------------------------------
//! Handle updates to the scene

void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    g_pGameRoot->OnFrameMove( fTime, fElapsedTime );
}

// --------------------------------------------------------------------------------------
//! Render the scene 

void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    if( g_SettingsDlg.IsActive() )
    {   // If the settings dialog is being shown, then render it instead of rendering the app's scene    
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }
    g_pGameRoot->OnFrameRender();
}

// --------------------------------------------------------------------------------------
//! Handle messages to the application 

LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_UI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    if(g_pGameRoot)
        *pbNoFurtherProcessing = g_pGameRoot->MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    return 0;
}


// --------------------------------------------------------------------------------------
//! Release resources created in the OnResetDevice callback here 

void CALLBACK OnLostDevice( void* pUserContext )
{    
    g_DialogResourceManager.OnLostDevice();
    g_SettingsDlg.OnLostDevice();    
    if( g_pGameRoot )
        g_pGameRoot->OnLostDevice();    
}


// --------------------------------------------------------------------------------------
//! Release resources created in the OnCreateDevice callback here

void CALLBACK OnDestroyDevice( void* pUserContext )
{    
    g_DialogResourceManager.OnDestroyDevice();
    g_SettingsDlg.OnDestroyDevice();

    if(g_pGameRoot) // could be deleted by CleanupApp().
        g_pGameRoot->OnDestroyDevice();
}


//--------------------------------------------------------------------------------------
//! Clean up resources allocated in InitApp()

void CleanupApp() 
{
    SAFE_DELETE( g_pGameRoot );    
}

//--------------------------------------------------------------------------------------
//! Renders any and all onscreen text
#if 0
void RenderText() {
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );
    const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );    
    txtHelper.DrawTextLine( DXUTGetFrameStats(true) );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );    

    // Draw help
    if( false )
    {
        txtHelper.SetInsertionPos( 10, pd3dsdBackBuffer->Height-15*21 );
        txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Controls (F1 to hide):" );

        txtHelper.SetInsertionPos( 40, pd3dsdBackBuffer->Height-15*20 );
        txtHelper.DrawTextLine( L"Camera Movement: W S A D\n"
                                L"Camera Rotation: Left Button drag mouse\n"                                
                                L"Save Clouds: F3\n"                                
                                L"Save NotBloomMap to File: F4\n"
                                L"\n"
                                L"Draw Terrain: F5\n"
                                L"Draw Clouds: F6\n"
                                L"Use PP Effects: F7\n"
                                L"Wireframe: F8\n"                                                                
                                L"Draw Bounding Boxes: F9\n"
                                L"View Sky Cam: F10\n"
                                L"Stop Clock (toggle): F11\n"
                                L"\n"
                                L"Cycle Through PP Effects: Insert\n"
                                L"\n"
                                L"Lock LOD 1: 1\n"
                                L"Lock LOD 2: 2\n"
                                L"Lock LOD 3: 3\n"                                
                                L"Unlock LOD: 0\n"
                                );

        txtHelper.SetInsertionPos( pd3dsdBackBuffer->Width - 200, pd3dsdBackBuffer->Height-15*20 );
        txtHelper.DrawTextLine( L"b:  Morph Into Black Sun\n"
                                L"n:  Morph Into Black Sky\n"
                                L"m:  Morph Into White Sky\n"
                                L",:  Return to Default Sun\n"
                                L"\n"
                                L"(alt)j:  (Decrease)Increase VSPower\n"
                                L"(alt)k:  (Decrease)Increase PSPower\n"
                                L"(alt)l:  (Decrease)Increase VSMult\n"
                                L"\n"
                                );                                
    }
    else
    {
        txtHelper.SetInsertionPos( 10, pd3dsdBackBuffer->Height-15*2 );
        txtHelper.SetForegroundColor( D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Press F1 for help" );
    }  
    txtHelper.End();
}
#endif

//--------------------------------------------------------------------------------------
//! Handles GUI ( buttons/HUD) Events 

void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    // WCHAR wszInput[MAX_PATH];    
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
    }
}

/*!
//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
*/
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    g_pGameRoot->KeyboardProc(nChar, bKeyDown, bAltDown);    
}

//--------------------------------------------------------------------------------------
//! Handle Mouse Events before being passed on

void CALLBACK MouseProc(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext)
{
	//TODO: Make it so clicking on the area where menu is only moves the camera 
	//		when the menu is hidden
    g_pGameRoot->MouseProc(bLeftButtonDown, 
                           bRightButtonDown, 
                           bMiddleButtonDown, 
                           bSideButton1Down, 
                           bSideButton2Down,
                           nMouseWheelDelta,
                           xPos,
                           yPos);
}

//--------------------------------------------------------------------------------------
//! Dumps System Information to Logger 

void DumpSystemToLog() {
    CPUInfo cpu;   
    WCHAR wszTime[9], wszDate[9];
    _wstrtime_s(wszTime, 9);
    _wstrdate_s(wszDate, 9);
    LOG_INFO(" --     System To Log     --");
#if defined(DEBUG) || defined(_DEBUG)
    LOG_INFO("RUNNING -DEBUG- BUILD AT: " << wszDate << " -- " << wszTime );
#else
    LOG_INFO("RUNNING -RELEASE- BUILD AT: " << wszDate << " -- " << wszTime );
#endif    
    LOG_INFO("");
    LOG_INFO(DXUTGetWindowTitle());
    LOG_INFO("Processor Speed: " << cpu.GetProcessorClockFrequency() << "MHz");
    LOG_INFO("Processors: " << cpu.GetLogicalProcessorsPerPhysical());
    LOG_INFO("Processor: " << cpu.GetExtendedProcessorName());
    LOG_INFO(DXUTGetFrameStats(false));
    LOG_INFO(DXUTGetDeviceStats());
    LOG_INFO("");

    dprintf("Processor Speed: %dMHz\n", cpu.GetProcessorClockFrequency());
    dprintf("Processors: %d\n", cpu.GetLogicalProcessorsPerPhysical());
    dprintf("Processor: %s\n", cpu.GetExtendedProcessorName());    

    char pszDest[256];
    WCharStringToCharString(DXUTGetFrameStats(false), pszDest, 255);
    dprintf("STATS:\n%s\n", pszDest);
    WCharStringToCharString(DXUTGetDeviceStats(), pszDest, 255);
    dprintf("%s\n", pszDest);
    dprintf("\n\n");
}

//--------------------------------------------------------------------------------------
//! Dumps Capabilities to Logger 

void DumpCapsToLog() {
    const D3DCAPS9 *caps = DXUTGetDeviceCaps();
    LOG_INFO(" --     CAPS To Log      --");
    LOG_INFO("PixelShaderVersion: " << setbase(16) << caps->PixelShaderVersion << " hex");
    LOG_INFO("VertexShaderVersion: " << setbase(16) << caps->VertexShaderVersion << " hex");
    LOG_INFO("VertexTextureFilterCaps: " << setbase(16) << caps->VertexTextureFilterCaps << " hex");
    LOG_INFO("MaxPShaderInstructionsExecuted: " << caps->MaxPShaderInstructionsExecuted);
    LOG_INFO("MaxVShaderInstructionsExecuted: " << caps->MaxVShaderInstructionsExecuted);
    LOG_INFO("MaxPixelShader30InstructionSlots: " << caps->MaxPixelShader30InstructionSlots);
    LOG_INFO("MaxVertexShader30InstructionSlots: " << caps->MaxVertexShader30InstructionSlots);
    LOG_INFO("MaxVertexIndex: " << caps->MaxVertexIndex);
    LOG_INFO("");

    dprintf(" -= CAPS to LOG =- \n");
    dprintf("PixelShaderVersion: %x\n", caps->PixelShaderVersion);
    dprintf("VertexShaderVersion: %x\n", caps->VertexShaderVersion);
    dprintf("VertexTextureFilterCaps: %x\n", caps->VertexTextureFilterCaps);
    dprintf("\n\n");
}
