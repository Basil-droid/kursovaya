#pragma once 
#include "framework.h"
#include "Definers.h"
const wchar_t authorizationClassName[] = L"AUTHORIZATION";
const wchar_t authorizationWindowName[] = L"VasiaPupkin's - Авторизация";
class ApplicationClass;
class AuthorizationClass
{
public:
    HWND WindowHwnd;
    ApplicationClass* app;
    HWND LabelLoginHwnd;
    HWND LabelPasswordHwnd;
    HWND EditLoginHwnd;
    HWND EditPasswordHwnd;
    HWND BtnAuthorizationHwnd;
    HWND BtnRegistrationHwnd;

    static void RegisterWindow(HINSTANCE hInstance);
    static AuthorizationClass* CreateWND(HINSTANCE hInstance, ApplicationClass* app);

};

