#pragma once
#include "AuthorizationClass.h"
#include "RegistrationClass.h"
#include "ChatClass.h"
#include "FindUser.h"

void RegisterWNDClasses(HINSTANCE hInstance)
{
	AuthorizationClass::RegisterWindow(hInstance);
	RegistrationClass::RegisterWindow(hInstance);
	ChatClass::RegisterWindow(hInstance);
	FindUser::RegisterWindow(hInstance);
}
void Init(HINSTANCE hInstance)
{
	RegisterWNDClasses(hInstance);
}
