#include "Game.h"
#include "InputDevice.h"

Game::Game() 
{

}

void Game::CreateBackBuffer()
{

}

bool Game::Initialize()
{
	Display = new DisplayWin32(1000, 800, Name);

	MainCamera = new Camera(Transform3D(Vector3(0.0f, 0.0f, -15.0f), Vector3::Zero, Vector3::One), Display->ClientWidth, Display->ClientHeight);

	Input = new InputDevice(this);

	if (!InitializeGraphics())
	{
		return false;
	}

	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->Initialize();
	}

	return true;
}

void Game::DestroyResources()
{
	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->DestroyResources();
	}

	for (int i = 0; i < VertexShaders.size(); i++)
	{
		VertexShaders[i]->DestroyResources();
	}

	for (int i = 0; i < PixelShaders.size(); i++)
	{
		PixelShaders[i]->DestroyResources();
	}

	for (int i = 0; i < Textures.size(); i++)
	{
		Textures[i]->DestroyResources();
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

	Context->OMSetRenderTargets(1, &RenderView, depthStencilView);

	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	Context->ClearRenderTargetView(RenderView, color);
	Context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);


	//Draw components
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Draw();
	}

	ImGUI.Draw();

	Context->OMSetRenderTargets(0, nullptr, nullptr);
	SwapChain->Present(1, 0);
}

void Game::EndFrame()
{

}

void Game::Exit()
{

}

void Game::Pause()
{
	isPauseRequested = true;
}

void Game::MessageHandler()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			InputDevice::KeyboardInputEventArgs args{
				0,
				msg.message,
				msg.wParam,
				msg.message
			};
			Input->OnKeyDown(args);
		}
		if (msg.message == WM_MOUSEMOVE)
		{
			InputDevice::RawMouseEventArgs args
			{
				/*MOUSE_MOVE_RELATIVE*/
				msg.message,
				msg.wParam,
				0,
				0,
				0,
				LOWORD(msg.lParam),
				HIWORD(msg.lParam)
			};
			Input->OnMouseMove(args);
		}

		if(msg.message == WM_MOUSEHOVER)

		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Game::PrepareFrame()
{

}

bool Game::InitializeGraphics()
{
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

	HRESULT res;

	res = D3D11CreateDeviceAndSwapChain(
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
		&Context
	);

	if (FAILED(res))
	{
		std::cout << "Failed to create device and swapchain" << std::endl;
		return false;
	}

	res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	if (FAILED(res))
	{
		std::cout << "Failed to create back buffer" << std::endl;
		return false;
	}

	res = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);

	if (FAILED(res))
	{
		std::cout << "Failed to create RenderTargetView" << std::endl;
		return false;
	}

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = Device->CreateRasterizerState(&rastDesc, &rastState);

	if (FAILED(res))
	{
		std::cout << "Failed to create rasterization state" << std::endl;
		return false;
	}

	Context->RSSetState(rastState);

	//Create depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = Display->ClientWidth;
	depthStencilDesc.Height = Display->ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	res = Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);

	if (FAILED(res))
	{
		std::cout << "Error to create depthStencilBuffer texture" << std::endl;
	}

	res = Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);

	if (FAILED(res))
	{
		std::cout << "Error to create depthStencilView" << std::endl;
	}

	//Create shaders
	for (int i = 0; i < VertexShaders.size(); i++)
	{
		if(!VertexShaders[i]->Initialize(Device.Get()))
		{
			return false;
		}
	}

	for (int i = 0; i < PixelShaders.size(); i++)
	{
		if (!PixelShaders[i]->Initialize(Device.Get()))
		{
			return false;
		}
	}

	//Create textures
	for (int i = 0; i < Textures.size(); i++)
	{
		if (FAILED(Textures[i]->Initialize(Device.Get())))
		{
			std::cout << "Texture " << i << " init failed" << std::endl;
			return false;
		}
	}

	ImGUI.Initialize(Device.Get(), Context, Display->hWnd);

	return true;
}

void Game::RestoreTarget()
{

}

void Game::Run()
{
	if (!Initialize())
	{
		return;
	}

	prevTime = std::chrono::steady_clock::now();

	while (!isExitRequested) 
	{
		auto curTime = std::chrono::steady_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
		prevTime = curTime;

		MessageHandler();
		UpdateInternal();
		Update();
		Draw();

		if (isPauseRequested) 
		{
			system("pause");
			isPauseRequested = false;
		}
	}

	DestroyResources();
}

void Game::Update()
{
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Update();
	}
}

void Game::UpdateInternal()
{
	//Update components
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->FixedUpdate();
	}

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
