#include "Client.h"


bool Client::Connect()
{

    if (int sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
        MessageBox(NULL, L"Error : Could not create socket", 0, 0);
    }

    int nSizeOfADDR = sizeof(_addr);

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(_socket, (SOCKADDR*)&_addr, sizeof(_addr)) != 0)
    {
        MessageBox(NULL, L"Error: failed connect to server", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }
    else
    {
        return true;
    }
}

SOCKET Client::GetSocket()
{
    return _socket;
}

int Client::SendData(const char* msg, int size)
{
    if (send(_socket, msg, size, 0) == SOCKET_ERROR) {       //Отправляем сообщение
        int num = WSAGetLastError();
        MessageBox(NULL, L"Error sending message", 0, 0);
        return 0;
    }
    else
        return 1;
}
