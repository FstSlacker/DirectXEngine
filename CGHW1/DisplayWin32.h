#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include "Graphics/ImGui/imgui_impl_win32.h"

class DisplayWin32
{
public:
	int ClientWidth;
	int ClientHeight;
	HWND hWnd;

	DisplayWin32(int width, int height, LPCWSTR name);

private:
	HINSTANCE hInstance;
	WNDCLASSEX wc;

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

