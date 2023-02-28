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

#include "Graphics.h"
#include "DisplayWin32.h"
#include "InputDevice.h"
#include "Collision.h"
#include "ImGuiManager.h"
#include "ImGuiDebugWindows.h"

#include "ImGuiPlanetSystem.h"

#include "GameComponent.h"
#include "MeshComponent.h"
#include "PlaneComponent.h"
#include "Text2DComponent.h"
#include "Camera.h"
#include "CameraMoveComponent.h"

#include "PlanetComponent.h"
#include "PlanetSystemComponent.h"
#include "SphereComponent.h"


class Game
{

public:
	std::vector<GameComponent*> Components;

	Graphics Gfx;
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

	bool isExitRequested = false;
	bool isPauseRequested = false;

	void DestroyResources();
	void Draw();
	bool Initialize();
	void MessageHandler();
	void Update();
	void UpdateInternal();
};

