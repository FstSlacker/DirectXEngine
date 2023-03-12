#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include "Graphics/ImGui/Source/imgui_impl_win32.h"

class Game;

class DisplayWin32
{
public:
	int ClientWidth;
	int ClientHeight;
	HWND hWnd;

	DisplayWin32(Game& game, int width, int height, LPCWSTR name);

private:
	HINSTANCE hInstance;
	WNDCLASSEX wc;
	Game* game;

	LRESULT HandleWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	static LRESULT CALLBACK SetupWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK RedirectWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

