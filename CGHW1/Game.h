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

#include "Logs.h"
#include "DisplayWin32.h"
#include "Input/InputDevice.h"
#include "ResourceManager.h"

#include "Graphics/Graphics.h"
#include "Graphics/RenderQueuePass.h"
#include "Graphics/ShadowMapPass.h"
#include "Graphics/CascadeShadowMapPass.h"
#include "Graphics/ImGui/ImGuiManager.h"
#include "Graphics/ImGui/ImGuiDebugWindows.h"
#include "Graphics/Gizmos/DebugGizmos.h"

#include "Components/Camera.h"
#include "Components/Light.h"
#include "Components/Collision.h"

#include "Components/GameComponent.h"
#include "Components/MeshComponent.h"
#include "Components/PlaneComponent.h"
#include "Components/Text2DComponent.h"
#include "Components/CameraMoveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CubeComponent.h"
#include "Components/ModelComponent.h"


class Game
{

public:
	std::vector<GameComponent*> Components;
	Light Light;

	Graphics Gfx;
	DisplayWin32* Display;
	InputDevice* Input;

	ResourceManager Resources;

	ImGuiManager ImGUI;
	DebugGizmos Gizmos;

	LPCWSTR Name;
	Game* Instance;
	float TotalTime;
	float DeltaTime;

	Game();
	void Run();
	void Exit();
	void Pause();
	void RestoreTarget();
	bool ResizeWindow(UINT width, UINT height);

private:
	std::chrono::time_point<std::chrono::steady_clock> prevTime;

	RenderQueuePass renderQueuePass;
	ShadowMapPass shadowMapPass;
	CascadeShadowMapPass cascadeShadowMapPass;


	bool isExitRequested = false;
	bool isPauseRequested = false;
	bool isInitialized = false;

	bool Initialize();
	void MessageHandler();
	void Update();
	void UpdateInternal();
	void Draw();
	void DestroyResources();
};

