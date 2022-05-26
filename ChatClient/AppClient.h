#pragma once
#include "Client.h"




// Êëàññ êëèåíòà íàøåé èãðû
class ApplicationClass;
class AppClient : Client
{
public:
	ApplicationClass* App;

	AppClient(ApplicationClass* application, char* ip, int port) : Client(ip, port)
	{
		App = application;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&AppClient::MessageHandler, this, 0, 0);
	}

	static void MessageHandler(LPVOID LParam);

	int LoginUser(wchar_t* login, wchar_t* password);
	int RegisterUser(wchar_t* login, wchar_t* password, wchar_t* name);
	int SendMessageToUser(wchar_t* mess, int idTo);
	int FindByLogin(wchar_t* login);

};

