#include "ApplicationClass.h"

ApplicationClass* ApplicationClass::CreateApp(HINSTANCE hInstance) 
{
	ApplicationClass* aplClass = new ApplicationClass();
	aplClass->autorization = AuthorizationClass::CreateWND(hInstance, aplClass);
	aplClass->registration = RegistrationClass::CreateWND(hInstance, aplClass);
	aplClass->chat = ChatClass::CreateWND(hInstance, aplClass);
	aplClass->findUser = FindUser::CreateWND(hInstance, aplClass);

	aplClass->appClient = new AppClient(aplClass, (char*)SERVER_IP, SERVER_PORT);
	aplClass->ShowAuthorizationWND();
	return aplClass;
}
void ApplicationClass::ShowAuthorizationWND()
{
	ShowWindow(autorization->WindowHwnd, SW_SHOW); // SW_SHOW
	ShowWindow(registration->WindowHwnd, SW_HIDE); // SW_HIDE
	ShowWindow(chat->WindowHwnd, SW_HIDE);// SW_HIDE
}
void ApplicationClass::ShowRegistrationWND()
{
	ShowWindow(autorization->WindowHwnd, SW_HIDE);
	ShowWindow(registration->WindowHwnd, SW_SHOW);
	ShowWindow(chat->WindowHwnd, SW_HIDE);
}
void ApplicationClass::ShowChatWND()
{
	int registerSuccess = MessageBox(
		NULL,
		L"����������� ������ �������",
		L"�����������",
		MB_OK
	);
	switch (registerSuccess)
	{
	default:
		ShowWindow(autorization->WindowHwnd, SW_HIDE);
		ShowWindow(registration->WindowHwnd, SW_HIDE);
		ShowWindow(chat->WindowHwnd, SW_SHOW);
		break;
	}
}
void ApplicationClass::ShowFindUserWND()
{
	ShowWindow(findUser->WindowHwnd, SW_SHOW);
}
void ApplicationClass::ShowUserRegistered()
{
	int registerSuccess = MessageBox(
		NULL,
		L"����������� ������ �������",
		L"�����������",
		MB_OK
	);
	switch (registerSuccess)
	{
		default:
			this->ShowAuthorizationWND();
			break;
	}
}
void ApplicationClass::ShowRegisterError()
{
	int registerError = MessageBox(
		NULL,
		L"������������ � ����� ������� ��� ���� � �������",
		L"������ �����������",
		MB_OK | MB_ICONERROR
	);
	switch (registerError)
	{
	default:
		break;
	}
}
void ApplicationClass::ShowAuthError()
{
	int registerError = MessageBox(
		NULL,
		L"�������� ����� / ������",
		L"������ �����������",
		MB_OK | MB_ICONERROR
	);
	switch (registerError)
	{
	default:
		break;
	}
}
void ApplicationClass::ShowFindError()
{
	int findError = MessageBox(
		NULL,
		L"������������ � ����� ������� �� ������",
		L"������ ������",
		MB_OK | MB_ICONERROR
	);
	switch (findError)
	{
	default:
		break;
	}
}
void ApplicationClass::ShowUserFinded()
{
	int findSuccess = MessageBox(
		NULL,
		L"������������ ������",
		L"�����������",
		MB_OK
	);
	switch (findSuccess)
	{
	default:
		ShowWindow(this->findUser->WindowHwnd, SW_HIDE);
		break;
	}
}

