#pragma once
#include "Client.h"


//enum COMMANDS { BROKE = 0, WAIT = 1, STARTGAME = 2, GAMERUNTIME = 3, ENDGAME = 4 };
//enum WAITCOMMANDS { WAITFORSTART = 1 };
//enum STARTGAMECOMMANDS { START = 1 };
//enum GAMERUNTIMECOMMANDS { SQUARECLICK = 1 };
//enum ENDGAMECOMMANDS { WIN = 1, LOSE = 2, ENEMYLOSTCONNECTION = 3, DRAW = 4, INTERRUPT = 5 };

// Класс клиента нашей игры
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

