#include "dxstdafx.h"
#include "splashScreen.h"

#define LOAD_SCREEN_IMG L"../Media/loading.bmp"
HBITMAP image;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_CREATE:
        image = (HBITMAP) LoadImage(NULL, LOAD_SCREEN_IMG, IMAGE_BITMAP, 250, 200, LR_LOADFROMFILE);
        //image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LOAD_SCREEN_IMG));
        if(image == NULL)
            MessageBox(hwnd, L"Could not load Load Screen image!", L"Error", MB_OK | MB_ICONEXCLAMATION);
        break;
    case WM_PAINT:
    {
        BITMAP bm;
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, image);

        GetObject(image, sizeof(bm), &bm);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);
        break;
    }    
    case WM_DESTROY:
        DeleteObject(image);        
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

SplashScreen::SplashScreen(int x, int y)
{
    width = 250;
    height = 200;
    
    image = NULL;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = NULL;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = L"hello!!";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        L"hello!!",
        L"The title of my window",
        WS_POPUP,
        x - width/2, y - height/2, width, height,
        NULL, NULL, NULL, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);
}

SplashScreen::~SplashScreen()
{
    DestroyWindow(hwnd);
}

void SplashScreen::display()
{
}