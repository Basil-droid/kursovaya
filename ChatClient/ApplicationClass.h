#pragma once
#include "AuthorizationClass.h"
#include "RegistrationClass.h"
#include "ChatClass.h"
#include "AppClient.h"
#include "FindUser.h"

class ApplicationClass
{
public:
	AuthorizationClass* autorization;
	RegistrationClass*	registration;
	ChatClass*			chat;
	AppClient*			appClient;
	FindUser*			findUser;

	static ApplicationClass* CreateApp(HINSTANCE hInstance);
	void ShowAuthorizationWND();
	void ShowRegistrationWND();
	void ShowFindUserWND();
	void ShowChatWND();
	void ShowUserRegistered();
	void ShowRegisterError();
	void ShowAuthError();
	void ShowUserFinded();
	void ShowFindError();
};

