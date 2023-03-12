#include "DisplayWin32.h"
#include "Game.h"

DisplayWin32::DisplayWin32(Game& game, int width, int height, LPCWSTR name)
{
	this->game = &game;

	ClientWidth = width;
	ClientHeight = height;

	hInstance = GetModuleHandle(nullptr);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = SetupWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_EXCLAMATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = name;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, name, name,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, 
		nullptr, 
		hInstance, 
		this
	);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
LRESULT DisplayWin32::HandleWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return true;

	switch (umessage)
	{
		case WM_SIZE:
		{
			this->ClientWidth = LOWORD(lparam);
			this->ClientHeight = HIWORD(lparam);
			game->ResizeWindow(this->ClientWidth, this->ClientHeight);
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}


LRESULT CALLBACK DisplayWin32::SetupWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const createPtr = reinterpret_cast<CREATESTRUCTW*>(lparam);
			DisplayWin32* windowPtr = reinterpret_cast<DisplayWin32*>(createPtr->lpCreateParams);
			if (windowPtr == nullptr)
			{
				Logs::LogError("Failed to get pointer for the window");
				exit(-1);
			}
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(RedirectWndProc));
			return windowPtr->HandleWndProc(hwnd, umessage, wparam, lparam);
		}
		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}

	switch (umessage)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			return 0;
		}
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			//std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
			return 0;
		}
		case WM_SIZE:
		{
			Logs::Log("Resize1", false);
			return 0;
		}
		default:
		{
			//PostMessage(hwnd, umessage, wparam, lparam);
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

LRESULT CALLBACK DisplayWin32::RedirectWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			return 0;
		}
		default:
		{
			DisplayWin32* const windowPtr = reinterpret_cast<DisplayWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			return windowPtr->HandleWndProc(hwnd, umessage, wparam, lparam);
		}
	}
}
