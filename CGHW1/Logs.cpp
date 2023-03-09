#include "Logs.h"
#include <comdef.h>

void Logs::Log(std::string msg, bool showMsgBox)
{
	PrintToConsole(msg.c_str());

	if (showMsgBox)
		ShowMsgBox(msg.c_str(), "Message", MB_ICONINFORMATION);
}

void Logs::LogError(HRESULT hr, std::string msg, bool showMsgBox)
{
	_com_error hrInfo(hr);
	std::wstring msgText = StringConverter::StringToWide(msg) + L"\n" + hrInfo.ErrorMessage();

	PrintToConsole(msg.c_str());

	if (showMsgBox)
		ShowMsgBox(msgText.c_str(), L"Error", MB_ICONERROR);
}

void Logs::LogIfError(HRESULT hr, std::string msg, bool showMsgBox)
{
	if (FAILED(hr))
	{
		LogError(hr, msg, showMsgBox);
	}
}

void Logs::ShowMsgBox(LPCWSTR text, LPCWSTR title, UINT type)
{
	MessageBoxW(nullptr, text, title, type);
}

void Logs::ShowMsgBox(LPCSTR text, LPCSTR title, UINT type)
{
	MessageBoxA(nullptr, text, title, type);
}

void Logs::PrintToConsole(const char* msg)
{
	std::cout << "Log: " << msg << std::endl;
}


