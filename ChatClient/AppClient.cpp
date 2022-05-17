#include "AppClient.h"
#include "ApplicationClass.h"

int AppClient::LoginUser(wchar_t* login, wchar_t* password)
{
    char message[MESSAGE_SIZE]{};
    message[0] = 2;
    message[1] = 0;
    memcpy(&message[2], login, wcslen(login) * sizeof(wchar_t));
    memcpy(&message[44], password, wcslen(password) * sizeof(wchar_t));

    SendData(message);
    return 0;
}
int AppClient::RegisterUser(wchar_t* login, wchar_t* password, wchar_t* name)
{
    char message[MESSAGE_SIZE]{};
    message[0] = 1;
    message[1] = 0;
    memcpy(&message[2], login, wcslen(login) * sizeof(wchar_t));
    memcpy(&message[44], password, wcslen(password) * sizeof(wchar_t));
    memcpy(&message[86], name, wcslen(name) * sizeof(wchar_t));
    SendData(message);
    return 0;
}
int AppClient::SendMessageToUser(wchar_t* mess, int idTo)
{
    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 0;
    memcpy(&message[2], &idTo, sizeof(int));
    memcpy(&message[22], mess, wcslen(mess) * sizeof(wchar_t));
    SendData(message);
    return 0;
}
int AppClient::FindByLogin(wchar_t* login)
{
    char message[MESSAGE_SIZE]{};
    message[0] = 3;
    message[1] = 1;
    memcpy(&message[2], login, wcslen(login) * sizeof(wchar_t));
    SendData(message);
    return 0;
}


void AppClient::MessageHandler(LPVOID LParam)
{
    AppClient* client = (AppClient*)LParam;
    char data[MESSAGE_SIZE];
    int command, command1;

    while (true) {
        memset(data, '\0', MESSAGE_SIZE);
        if (recv(client->GetSocket(), data, sizeof(data), 0))
        {
            command = data[0]; command1 = data[1];
            if (command == 1)      // Регистрация
            {
                if (command1 == 0) // Успешно
                {
                    client->App->ShowUserRegistered();
                }
                if (command1 == 1) // Занят логин
                {
                    client->App->ShowRegisterError();
                }
            }
            if (command == 2)      // Авторизация
            {
                if (command1 == 0) // Успешно
                {
                    client->App->ShowChatWND();
                }
                if (command1 == 1) // Неправильный логин/пароль 
                {
                    client->App->ShowAuthError();
                }
            }
            if (command == 3)       // работа с чатом
            {
                if (command1 == 0)  // добавление пользователя в список
                {
                    wchar_t name[20];
                    memcpy(name, &data[22], 40);
                    char buffer[20];
                    memcpy(buffer, &data[2], 20);
                    int id = *(int*)buffer;
                    client->App->chat->FillUsersContacts(id, name);
                }
                if (command1 == 1) // получение сообщения 
                {
                    char buffer[20];
                    int idFrom, idTo;
                    memcpy(buffer, &data[2], 20);
                    idFrom = *(int*)buffer;
                    memcpy(buffer, &data[22], 20);
                    idTo = *(int*)buffer;
                    wchar_t mess[INPUT_MESS_LENGHT];
                    memcpy(mess, &data[42], INPUT_MESS_LENGHT*sizeof(wchar_t));
                    idTo = *(int*)buffer;
                    client->App->chat->FillUsersMessages(idFrom, idTo, mess);
                }
                if (command1 == 2) // пользователь найден 
                {
                    client->App->ShowUserFinded();
                }
                if (command1 == 3) // неудачная попытка найти пользователя
                {
                    client->App->ShowFindError();
                }
            }
        };
    }
}
