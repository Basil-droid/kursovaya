#pragma once
#pragma once 
#include "framework.h"
#include "Definers.h"
const wchar_t findUserClassName[] = L"FINDUSER";
const wchar_t findUserWindowName[] = L"VasiaPupkin's";
class ApplicationClass;
class FindUser
{
public:
    HWND WindowHwnd;
    HWND LabelLoginHwnd;
    HWND EditLoginHwnd;
    HWND FindUserBtn;
    ApplicationClass* app;

    static void RegisterWindow(HINSTANCE hInstance);
    static FindUser* CreateWND(HINSTANCE hInstance, ApplicationClass* app);

};

