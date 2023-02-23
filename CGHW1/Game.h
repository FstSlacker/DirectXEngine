#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>

#include <iostream>
#include <vector>

#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include <chrono>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include "GameComponent.h"
#include "MeshComponent.h"
#include "PlaneComponent.h"
#include "RacketComponent.h"
#include "BallComonent.h"
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "Collision.h"

class GameComponent;
class MeshComponent;
class PlaneComponent;

class Game
{

public:
	ID3D11DeviceContext* Context;
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11RenderTargetView* RenderView;
	IDXGISwapChain* SwapChain;
	std::vector<GameComponent*> Components;
	DisplayWin32* Display;
	InputDevice* Input;
	

	LPCWSTR Name;
	Game* Instance;
	float TotalTime;
	float DeltaTime;

	Game();
	void Run();
	void Exit();
	void RestoreTarget();

private:
	std::chrono::time_point<std::chrono::steady_clock> prevTime;
	ID3D11Texture2D* backBuffer;
	ID3D11RasterizerState* rastState;
	bool isExitRequested = false;

	void CreateBackBuffer();
	void DestroyResources();
	void Draw();
	void EndFrame();
	void Initialize();
	void MessageHandler();
	void PrepareFrame();
	void PrepareResources();
	void Update();
	void UpdateInternal();
};

