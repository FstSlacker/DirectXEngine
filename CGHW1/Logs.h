#pragma once
#include <Windows.h>
#include <iostream>
#include "StringHelper.h"

class Logs
{
public:
	static void Log(std::string msg, bool showMsgBox = true);

	static void LogError(HRESULT hr, std::string msg, bool showMsgBox = true);

	static void LogIfError(HRESULT hr, std::string msg, bool showMsgBox = true);

private:
	static void ShowMsgBox(LPCWSTR text, LPCWSTR title, UINT type);
	static void ShowMsgBox(LPCSTR text, LPCSTR title, UINT type);
	static void PrintToConsole(const char* msg);
};

