#include "FindUser.h"
#include "ApplicationClass.h"

LRESULT CALLBACK FindUserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static ApplicationClass* app = nullptr;
    switch (uMsg)
    {
    case WM_CREATE:
        app = (ApplicationClass*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        return 0;
    case WM_DESTROY:
        return 0;
    case WM_COMMAND:
    {
        auto wmId = LOWORD(wParam);
        switch (wmId)
        {
        case FIND_FIND_BTN:
            wchar_t login[INPUT_LOGIN];
            GetWindowText(app->findUser->EditLoginHwnd, login, INPUT_LOGIN);
            app->appClient->FindByLogin(login);
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

        HBITMAP hBitmap01 = (HBITMAP)LoadImage(NULL, L"fon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


        hdc01 = BeginPaint(hwnd, &ps01);
        //hdc02 = BeginPaint(app->chat->MessageEditHwnd, &ps02);
        //SetBkMode(hdc02, TRANSPARENT);
        //SetTextColor(hdc02, RGB(255, 255, 0));
        //SetWindowLongW(app->chat->MessageEditHwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_DLGFRAME);
        hdcMem01 = CreateCompatibleDC(hdc01);
        oldBitmap01 = SelectObject(hdcMem01, hBitmap01);

        GetObject(hBitmap01, sizeof(bitmap01), &bitmap01);
        BitBlt(hdc01, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, SRCCOPY);

        SelectObject(hdcMem01, oldBitmap01);
        DeleteDC(hdcMem01);

        EndPaint(hwnd, &ps01);

        return 0;
    }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void FindUser::RegisterWindow(HINSTANCE hInstance) {

    WNDCLASS findUserWC = { };
    findUserWC.lpfnWndProc = FindUserWindowProc;
    findUserWC.hInstance = hInstance;
    findUserWC.lpszClassName = findUserClassName;
    findUserWC.style = CS_HREDRAW | CS_VREDRAW;
    findUserWC.cbClsExtra = 0;
    findUserWC.cbWndExtra = 0;
    findUserWC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    findUserWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    //authorizationWC.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&findUserWC);
}
FindUser* FindUser::CreateWND(HINSTANCE hInstance, ApplicationClass* app)
{
    FindUser* findUserCls = new FindUser();
    findUserCls->app = app;
    findUserCls->WindowHwnd = CreateWindowEx(
        0,                              // Optional window styles.
        findUserClassName,                     // Window class
        findUserWindowName,    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(app)        // Additional application data
    );
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");
    findUserCls->LabelLoginHwnd = CreateWindowEx(
        0,
        L"EDIT",
        L"Логин",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
        50, 50, 70, 25,
        findUserCls->WindowHwnd,
        NULL,
        hInstance,
        NULL
    );
    findUserCls->EditLoginHwnd = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT,
        150, 50, 200, 25,
        findUserCls->WindowHwnd,
        (HMENU)FIND_LOGIN_EDIT_WND,
        hInstance,
        NULL
    );
    findUserCls->FindUserBtn = CreateWindowEx(
        0,
        L"BUTTON",
        L"Найти",
        BS_CENTER | WS_CHILD | WS_VISIBLE,

        150, 130, 70, 40,

        findUserCls->WindowHwnd,
        (HMENU)FIND_FIND_BTN,
        hInstance,
        NULL
    );

    SendMessage(findUserCls->LabelLoginHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(findUserCls->EditLoginHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(findUserCls->FindUserBtn, WM_SETFONT, WPARAM(hFont), TRUE);
    return findUserCls;


}

