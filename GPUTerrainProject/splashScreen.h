#include <windows.h>

class SplashScreen
{
    double percentComplete;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    int width, height;    

public:
    SplashScreen(int x, int y);
    ~SplashScreen();

    //LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void update(double percent) {percentComplete = percent;}

    void display();
};