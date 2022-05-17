#pragma once

#pragma comment(lib,"Comctl32.lib") 
#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <Commctrl.h>
#include <windowsx.h>
#include <winsock.h>

const int MESSAGE_SIZE = 512;

// Базовый класс клиента
class Client
{
	SOCKADDR_IN _addr;
	SOCKET _socket;

public:
	Client(char* ip, int port)
	{
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData))
			MessageBox(NULL, L"Error: failed load library Winsock", L"Error", MB_OK | MB_ICONERROR);

		_addr.sin_addr.s_addr = inet_addr(ip);
		_addr.sin_port = htons(port);
		_addr.sin_family = AF_INET;

		Connect();
	}

	bool Connect();
	SOCKET GetSocket();
	int SendData(const char* msg, int size = MESSAGE_SIZE);
};

