#include "AuthorizationClass.h"
#include "ApplicationClass.h"

LRESULT CALLBACK AuthorizationWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static AuthorizationClass* auth = nullptr;
    static ApplicationClass* app = nullptr;
    switch (uMsg)
    {
    case WM_CREATE:
        app = (ApplicationClass*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND: {
        auto wmId = LOWORD(wParam);
        switch (wmId)
        {
        case REGISTRATIONBTN:
            app->ShowRegistrationWND();
            break;
        case AUTHORIZATIOBTN:
            wchar_t login[INPUT_LOGIN], password[INPUT_PASSWORD];
            GetWindowText(app->autorization->EditLoginHwnd, login, INPUT_LOGIN);
            GetWindowText(app->autorization->EditPasswordHwnd, password, INPUT_PASSWORD);
            app->appClient->LoginUser(login, password);
            break;
        default:
            break;
        return 0;
        }
        break; }
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


void AuthorizationClass::RegisterWindow(HINSTANCE hInstance) {

    WNDCLASS authorizationWC = { }; // окно авторизации
    authorizationWC.lpfnWndProc = AuthorizationWindowProc;
    authorizationWC.hInstance = hInstance;
    authorizationWC.lpszClassName = authorizationClassName;
    authorizationWC.style = CS_HREDRAW | CS_VREDRAW;
    authorizationWC.cbClsExtra = 0;
    authorizationWC.cbWndExtra = 0;
    authorizationWC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    authorizationWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    //authorizationWC.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&authorizationWC);
}
AuthorizationClass* AuthorizationClass::CreateWND(HINSTANCE hInstance, ApplicationClass* app)
{
    AuthorizationClass* authCls = new AuthorizationClass();
    authCls->app = app;
    authCls->WindowHwnd = CreateWindowEx(
        0,                              // Optional window styles.
        authorizationClassName,                     // Window class
        authorizationWindowName,    // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

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
    authCls->LabelLoginHwnd = CreateWindowEx(
        0,
        L"EDIT",
        L"Логин",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        100, 150, 70, 30,
        authCls->WindowHwnd,
        NULL,
        hInstance,
        NULL
    );
    authCls->LabelPasswordHwnd = CreateWindowEx(
        0,
        L"EDIT",
        L"Пароль",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        100, 250, 85, 30,
        authCls->WindowHwnd,
        NULL,
        hInstance,
        NULL
    );
    authCls->EditLoginHwnd = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER,
        300, 150, 200, 30,
        authCls->WindowHwnd,
        (HMENU)AUTHLOGINEDITWND,
        hInstance,
        NULL
    );
    authCls->EditPasswordHwnd = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_PASSWORD,
        300, 250, 200, 30,
        authCls->WindowHwnd,
        (HMENU)AUTHPASSWORDEDIT,
        hInstance,
        NULL
    );
    authCls->BtnAuthorizationHwnd = CreateWindowEx(
        0,                                  
        L"BUTTON",                          
        L"Войти",                           
        BS_CENTER | WS_CHILD | WS_VISIBLE,  

        100, 350, 100, 50,

        authCls->WindowHwnd,       // Parent window    
        (HMENU)AUTHORIZATIOBTN, // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    authCls->BtnRegistrationHwnd = CreateWindowEx(
        0,
        L"BUTTON",
        L"Зарегистрироваться",
        BS_CENTER | WS_CHILD | WS_VISIBLE,

        250, 350, 250, 50,

        authCls->WindowHwnd,   
        (HMENU)REGISTRATIONBTN,
        hInstance,  
        NULL        
    );

    SendMessage(authCls->LabelLoginHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(authCls->LabelPasswordHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(authCls->EditPasswordHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(authCls->EditLoginHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(authCls->BtnRegistrationHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(authCls->BtnAuthorizationHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    return authCls;


}
