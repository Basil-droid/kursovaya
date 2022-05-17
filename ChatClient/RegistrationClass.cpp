#include "RegistrationClass.h"
#include "ApplicationClass.h"

LRESULT CALLBACK RegistrationWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static RegistrationClass* reg = nullptr;
    static ApplicationClass* app = nullptr;
    switch (uMsg)
    {
    case WM_CREATE:
        app = (ApplicationClass*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;
    case WM_CLOSE:
        app->ShowAuthorizationWND();
        return 0;
    case WM_COMMAND: 
        {
        auto wmId = LOWORD(wParam);
        switch (wmId)
        {
        case REGISTERBTN:
            wchar_t login[INPUT_LOGIN], password[INPUT_PASSWORD], name[INPUT_NAME];
            GetWindowText(app->registration->EditLoginHwnd, login, INPUT_LOGIN);
            GetWindowText(app->registration->EditPasswordHwnd, password, INPUT_PASSWORD);
            GetWindowText(app->registration->EditNameHwnd, name, INPUT_NAME); // тут должно быть имя, позже исправить!
            app->appClient->RegisterUser(login, password, name);
            break;
        }
        return 0;
        }
    case WM_PAINT:
    {
        PAINTSTRUCT     ps01;
        HDC             hdc01;
        BITMAP          bitmap01;
        HDC             hdcMem01;
        HGDIOBJ         oldBitmap01;

        HBITMAP hBitmap01 = (HBITMAP)LoadImage(NULL, L"vasia.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


        hdc01 = BeginPaint(hwnd, &ps01);

        hdcMem01 = CreateCompatibleDC(hdc01);
        oldBitmap01 = SelectObject(hdcMem01, hBitmap01);

        GetObject(hBitmap01, sizeof(bitmap01), &bitmap01);
        BitBlt(hdc01, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, SRCCOPY);

        SelectObject(hdcMem01, oldBitmap01);
        DeleteDC(hdcMem01);

        EndPaint(hwnd, &ps01);
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void RegistrationClass::RegisterWindow(HINSTANCE hInstance) {

    WNDCLASS registrationWC = { }; // окно регистрации
    registrationWC.lpfnWndProc = RegistrationWindowProc;
    registrationWC.hInstance = hInstance;
    registrationWC.lpszClassName = registrationClassName;
    registrationWC.style = CS_HREDRAW | CS_VREDRAW;
    registrationWC.cbClsExtra = 0;
    registrationWC.cbWndExtra = 0;
    registrationWC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    registrationWC.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&registrationWC);
}
RegistrationClass* RegistrationClass::CreateWND(HINSTANCE hInstance, ApplicationClass* app)
{
    RegistrationClass* regCls = new RegistrationClass();
    regCls->app = app;
    regCls->WindowHwnd = CreateWindowEx(
        0,                              // Optional window styles.
        registrationClassName,                     // Window class
        registrationWindowName,    // Window text
        //WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 1150, 550,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(app)        // Additional application data
    );
    HFONT hFont = CreateFont(23, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");
    regCls->LabelLoginHwnd = CreateWindowEx(
        0,
        L"EDIT",
        L"Логин",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        100, 150, 70, 30,
        regCls->WindowHwnd,
        NULL,
        hInstance,
        NULL
    );
    regCls->LabelPasswordHwnd = CreateWindowEx(
        0,
        L"EDIT",
        L"Пароль",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        100, 250, 85, 30,
        regCls->WindowHwnd,
        NULL,
        hInstance,
        NULL
    );
    regCls->LabelNameHwnd = CreateWindowEx(
        0,
        L"EDIT",
        L"Имя",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        100, 350, 55, 30,
        regCls->WindowHwnd,
        NULL,
        hInstance,
        NULL
    );
    regCls->EditLoginHwnd = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER,
        300, 150, 200, 30,
        regCls->WindowHwnd,
        (HMENU)REGLOGINEDITWND,
        hInstance,
        NULL
    );
    regCls->EditPasswordHwnd = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER,
        300, 250, 200, 30,
        regCls->WindowHwnd,
        (HMENU)REGPASSWORDEDITWND,
        hInstance,
        NULL
    );

    regCls->EditNameHwnd = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER,
        300, 350, 200, 30,
        regCls->WindowHwnd,
        (HMENU)REGNAMEEDITWND,
        hInstance,
        NULL
    );
    
    regCls->BtnRegistrationHwnd = CreateWindowEx(
        0,
        L"BUTTON",
        L"Зарегистрироваться",
        BS_CENTER | WS_CHILD | WS_VISIBLE,

        200, 420, 200, 50,

        regCls->WindowHwnd,
        (HMENU)REGISTERBTN,
        hInstance,
        NULL
    );

    SendMessage(regCls->LabelLoginHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(regCls->LabelPasswordHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(regCls->EditPasswordHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(regCls->EditLoginHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(regCls->LabelNameHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(regCls->EditNameHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(regCls->BtnRegistrationHwnd, WM_SETFONT, WPARAM(hFont), TRUE);

    return regCls;


}
