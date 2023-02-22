#include "Game.h"

Game::Game() 
{

}

void Game::CreateBackBuffer()
{

}

void Game::Initialize()
{
	PrepareResources();

	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Initialize();
	}

}

void Game::DestroyResources()
{
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->DestroyResources();
	}

	SwapChain->Release();
	Device->Release();
	Context->Release();
	backBuffer->Release();
	RenderView->Release();
}

void Game::Draw()
{
	Context->ClearState();

	Context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	Context->RSSetViewports(1, &viewport);

	Context->OMSetRenderTargets(1, &RenderView, nullptr);

	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	Context->ClearRenderTargetView(RenderView, color);


	//Draw components
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Draw();
	}

	Context->OMSetRenderTargets(0, nullptr, nullptr);

	SwapChain->Present(1, 0);
}

void Game::EndFrame()
{

}

void Game::Exit()
{

}

void Game::MessageHandler()
{

}

void Game::PrepareFrame()
{

}

void Game::PrepareResources()
{
	Display = new DisplayWin32(800, 800, L"GameFramework");

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	res = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = Device->CreateRasterizerState(&rastDesc, &rastState);

	Context->RSSetState(rastState);
}

void Game::RestoreTarget()
{

}

void Game::Run()
{
	Initialize();

	prevTime = std::chrono::steady_clock::now();

	MSG msg = {};
	bool isExitRequested = false;
	while (!isExitRequested) {
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}

		auto curTime = std::chrono::steady_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
		prevTime = curTime;

		MessageHandler();
		UpdateInternal();
		Update();
		Draw();
	}

	DestroyResources();
}

float mul = 1;

void Game::Update()
{
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Update();
	}
}

void Game::UpdateInternal()
{
	//Handle collisions
	for (int i = 0; i < Components.size(); i++)
	{
		for (int j = 0; j < Components.size(); j++)
		{
			if (i == j)
				continue;

			ColliderBase* c1 = Components[i]->Collider;
			ColliderBase* c2 = Components[j]->Collider;

			if (c1 == nullptr || c2 == nullptr)
				continue;

			c1->HandleCollision(c2);
		}
	}
}
