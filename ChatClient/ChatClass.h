#pragma once
#include "framework.h"
#include "Definers.h"
#include "list"
const wchar_t chatClassName[] = L"CHAT";
const wchar_t chatWindowName[] = L"VasiaPupkin's";
using namespace std;
typedef struct
{
    wchar_t Name[INPUT_NAME];
    int id;
} User;

typedef struct
{
    wchar_t Mess[INPUT_MESS_LENGHT];
    int fromId, toID;
} Message;
class ApplicationClass;
void ChatWndAddMenus(HWND mainHwnd);
class ChatClass
{
public:
    HWND WindowHwnd;
    ApplicationClass* app;
    HWND UsersListHwnd;
    HWND ChatListHwnd;
    HWND MessageEditHwnd;
    HWND SendMessageBTN;
    list<User> users;
    list<User>::iterator i;
    list<Message> messages;
    list<Message>::iterator j;

    static void RegisterWindow(HINSTANCE hInstance);
    static ChatClass* CreateWND(HINSTANCE hInstance, ApplicationClass* app);
    void FillUsersContacts(int id, wchar_t* name);
    void FillUsersMessages(int fromId, int toId, wchar_t* mess);
    void FillEditBoxMessages(int id);
    wchar_t* GetContactName(int id);
};

