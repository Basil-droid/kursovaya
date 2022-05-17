#include "ChatClass.h"
#include "ApplicationClass.h"


LRESULT CALLBACK ChatWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int currentUser;
    static ApplicationClass* app = nullptr;
    switch (uMsg)
    {
        case WM_CREATE:
            app = (ApplicationClass*)((LPCREATESTRUCT)lParam)->lpCreateParams;
            ChatWndAddMenus(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND: 
        {
            auto wmId = LOWORD(wParam);
            switch (wmId)
            {
                case FIND_USER_MENU:
                    app->ShowFindUserWND();
                    break;
                case EXIT_FROM_CHAT:
                    MessageBox(NULL, L"Выходим", L"Выходимм", MB_OK);
                    break;
                case MESSAGE_SEND_BTN:
                {
                    auto wmHID = HIWORD(wParam);
                    switch (wmHID)
                    {
                        case BN_CLICKED:
                        {
                            wchar_t mess[INPUT_MESS_LENGHT];
                            GetWindowTextW(app->chat->MessageEditHwnd, mess, INPUT_MESS_LENGHT);

                            int lbItem = (int)SendMessage(app->chat->UsersListHwnd, LB_GETCURSEL, 0, 0);
                            int id = (int)SendMessage(app->chat->UsersListHwnd, LB_GETITEMDATA, lbItem, 0);
                            app->appClient->SendMessageToUser(mess, id);
                            break;
                        }
                    }
                    break;
                }
                case USERS_LIST_ID_WND:
                {

                    auto wmHID = HIWORD(wParam);
                    switch (wmHID)
                    {
                        case LBN_SELCHANGE:
                        {
                            int lbItem = (int)SendMessage(app->chat->UsersListHwnd, LB_GETCURSEL, 0, 0);
                            int i = (int)SendMessage(app->chat->UsersListHwnd, LB_GETITEMDATA, lbItem, 0);
                            if (i != currentUser && i != -1)
                            {
                                app->chat->FillEditBoxMessages(i);
                                currentUser = i;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            return 0;
        }
        /*case WM_CTLCOLOREDIT:
        {
            if ((HWND)lParam == app->chat->ChatListHwnd)
            {
                int a = 64;
                SetTextColor((HDC)wParam, RGB(255, 255, 255));
                SetBkColor((HDC)wParam, RGB(a, a, a));
                return (DWORD)CreateSolidBrush(RGB(a, a, a));
            }
            if ((HWND)lParam == app->chat->MessageEditHwnd)
            {
                int a = 255;
                SetTextColor((HDC)wParam, RGB(255, 255, 255));
                SetBkColor((HDC)wParam, RGB(a, a, a));
                return (DWORD)CreateSolidBrush(RGB(a, a, a));
            }
        }*/
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

void ChatClass::RegisterWindow(HINSTANCE hInstance) {

    WNDCLASS chatWC = { }; // окно авторизации
    chatWC.lpfnWndProc = ChatWindowProc;
    chatWC.hInstance = hInstance;
    chatWC.lpszClassName = chatClassName;
    chatWC.style = CS_HREDRAW | CS_VREDRAW;
    chatWC.cbClsExtra = 0;
    chatWC.cbWndExtra = 0;
    chatWC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    chatWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    //authorizationWC.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClass(&chatWC);
}
ChatClass* ChatClass::CreateWND(HINSTANCE hInstance, ApplicationClass* app)
{
    ChatClass* chatCls = new ChatClass();
    chatCls->app = app;
    chatCls->WindowHwnd = CreateWindowEx(
        0,                              // Optional window styles.
        chatClassName,                     // Window class
        chatWindowName,    // Window text
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 750, 600,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        static_cast<LPVOID>(app)        // Additional application data
    );
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial");
    chatCls->UsersListHwnd = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        L"LISTBOX",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY,
        50, 40, 200, 480,
        chatCls->WindowHwnd,
        (HMENU)USERS_LIST_ID_WND,
        hInstance,
        NULL);
    chatCls->ChatListHwnd = CreateWindowExW(
        WS_EX_WINDOWEDGE,
        WC_EDIT,
        L"",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
        300, 40, 380, 350,
        chatCls->WindowHwnd,
        (HMENU)CHAT_LIST_ID_WND,
        hInstance,
        NULL);
    //(WS_EX_STATICEDGE, WC_EDIT, NULL, WS_CHILD | WS_DLGFRAME, 36, 5, 1219, 25, hWnd, (HMENU)999, hInst, NULL)
    chatCls->MessageEditHwnd = CreateWindowExW(
        WS_EX_WINDOWEDGE,
        WC_EDIT,
        L"",
        WS_CHILD|  WS_VISIBLE | ES_MULTILINE ,
        300, 420, 290, 100,
        chatCls->WindowHwnd,
        (HMENU)MESSAGE_EDIT_WND,
        hInstance,
        NULL);
    chatCls->SendMessageBTN = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        L"BUTTON",
        L"Отправить",
        WS_CHILD | WS_VISIBLE ,
        600, 420, 80, 50,
        chatCls->WindowHwnd,
        (HMENU)MESSAGE_SEND_BTN,
        hInstance,
        NULL);

    SendMessage(chatCls->SendMessageBTN, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(chatCls->MessageEditHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(chatCls->ChatListHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    SendMessage(chatCls->UsersListHwnd, WM_SETFONT, WPARAM(hFont), TRUE);
    return chatCls;
};
void ChatWndAddMenus(HWND hwnd) 
{
    HMENU rootMenu = CreateMenu();
    HMENU subMenu = CreateMenu();

    AppendMenu(subMenu, MF_STRING, FIND_USER_MENU, L"Добавить контакт");
    AppendMenu(subMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(subMenu, MF_STRING, EXIT_FROM_CHAT, L"Выйти");

    AppendMenu(rootMenu, MF_POPUP, (UINT_PTR)subMenu, L"Главная");
    SetMenu(hwnd, rootMenu);
}
void ChatClass::FillUsersContacts(int id, wchar_t* name)
{
    User usr;
    wcscpy_s(usr.Name, name);
    usr.id = id;
    users.push_back(usr);
    int pos = (int)SendMessage(UsersListHwnd, LB_ADDSTRING, 0, (LPARAM)usr.Name);
    SendMessage(UsersListHwnd, LB_SETITEMDATA, pos, (LPARAM)usr.id);
    SetFocus(UsersListHwnd);
}
void ChatClass::FillUsersMessages(int fromId, int toId, wchar_t* mess) 
{
    Message msg;
    wcscpy_s(msg.Mess, mess);
    msg.fromId = fromId;
    msg.toID = toId;
    messages.push_back(msg);
    int lbItem = (int)SendMessage(UsersListHwnd, LB_GETCURSEL, 0, 0);
    int i = (int)SendMessage(UsersListHwnd, LB_GETITEMDATA, lbItem, 0);
    wchar_t name[INPUT_NAME];

    if (i == fromId)
    {
        wcscpy_s(name, GetContactName(i));
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(name));
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L": "));
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(msg.Mess));
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L"\r\n"));
    }
    if (i == toId)
    {
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L"Вы: "));
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(msg.Mess));
        SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L"\r\n"));
    }
}
wchar_t* ChatClass::GetContactName(int id) 
{
    for (i = users.begin(); i != users.end(); i++)
    {
        if ((*i).id == id)
        {
            return (*i).Name;
        }
    }
}
void ChatClass::FillEditBoxMessages(int id)
{
    static wchar_t name[INPUT_NAME];
    SetWindowText(ChatListHwnd, L"");
    wcscpy_s(name, GetContactName(id));
    for (j = messages.begin(); j != messages.end(); j++)
    {
        if ((*j).fromId == id)
        {
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(name));
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L": "));
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>((*j).Mess));
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L"\r\n"));
        }
        if ((*j).toID == id)
        {
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L"Вы: "));
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>((*j).Mess));
            SendMessage(ChatListHwnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(L"\r\n"));
        }
    }
}