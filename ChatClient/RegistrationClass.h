#pragma once
#include "framework.h"
#include "Definers.h"
const wchar_t registrationClassName[] = L"REGISTRATION";
const wchar_t registrationWindowName[] = L"VasiaPupkin's - Регистрация";
class ApplicationClass;
class RegistrationClass
{
public:
    HWND WindowHwnd;
    ApplicationClass* app;
    HWND LabelLoginHwnd;
    HWND LabelPasswordHwnd;
    HWND LabelNameHwnd;
    HWND EditLoginHwnd;
    HWND EditPasswordHwnd;
    HWND EditNameHwnd;
    HWND BtnRegistrationHwnd;

    static void RegisterWindow(HINSTANCE hInstance);
    static RegistrationClass* CreateWND(HINSTANCE hInstance, ApplicationClass* app);
};

