#ifndef UNICODE
#define UNICODE
#endif 
#include <windows.h>
#include "Initializer.h"
#include "ApplicationClass.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Init(hInstance); // регистрация всех исп окон
    ApplicationClass* app = ApplicationClass::CreateApp(hInstance);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}