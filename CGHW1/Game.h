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

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "Collision.h"
#include "Shaders.h"
#include "ImGuiManager.h"
#include "ImGuiDebugWindows.h"

#include "GameComponent.h"
#include "MeshComponent.h"
#include "PlaneComponent.h"
#include "Text2DComponent.h"
#include "Camera.h"
#include "CameraMoveComponent.h"

#include "PlanetComponent.h"
#include "PlanetSystemComponent.h"
#include "SphereComponent.h"

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

	std::vector<VertexShader*> VertexShaders;
	std::vector<PixelShader*> PixelShaders;
	std::vector<Texture*> Textures;

	DisplayWin32* Display;
	InputDevice* Input;
	ImGuiManager ImGUI;

	Camera* MainCamera;

	LPCWSTR Name;
	Game* Instance;
	float TotalTime;
	float DeltaTime;

	Game();
	void Run();
	void Exit();
	void Pause();
	void RestoreTarget();

private:
	std::chrono::time_point<std::chrono::steady_clock> prevTime;

	ID3D11Texture2D* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* rastState;
	bool isExitRequested = false;
	bool isPauseRequested = false;

	void CreateBackBuffer();
	void DestroyResources();
	void Draw();
	void EndFrame();
	bool Initialize();
	void MessageHandler();
	void PrepareFrame();
	bool InitializeGraphics();
	void Update();
	void UpdateInternal();
};

