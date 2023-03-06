#include "Graphics.h"

bool Graphics::Initialize(HWND hWnd, UINT width, UINT height)
{
	this->displayWidth = width;
	this->displayHeight = height;

    if (!InitializeDirectX(hWnd))
    {
        std::cout << "Error init directx" << std::endl;
        return false;
    }
	if (!InitializeResources())
	{
		std::cout << "Error init graphics resources" << std::endl;
		return false;
	}

    return true;
}

void Graphics::DestroyResources()
{
	for (int i = 0; i < vertexShaders.size(); i++)
	{
		vertexShaders[i]->DestroyResources();
	}

	for (int i = 0; i < pixelShaders.size(); i++)
	{
		pixelShaders[i]->DestroyResources();
	}

	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->DestroyResources();
	}

	swapChain->Release();
	device->Release();
	context->Release();
	backBuffer->Release();
	renderView->Release();
}

void Graphics::PrepareFrame()
{
	context->ClearState();

	context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(displayWidth);
	viewport.Height = static_cast<float>(displayHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	context->OMSetRenderTargets(1, &renderView, depthStencilView);

	float color[] = { BackgroundColor.x, BackgroundColor.y, BackgroundColor.z, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void Graphics::EndFrame()
{
	context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, 0);
}

void Graphics::AddVertexShader(VertexShader* vs)
{
	vertexShaders.push_back(vs);
}

void Graphics::AddPixelShader(PixelShader* ps)
{
	pixelShaders.push_back(ps);
}

void Graphics::AddTexture(Texture* tex)
{
	textures.push_back(tex);
}

ID3D11DeviceContext* Graphics::GetContext() const
{
	return this->context;
}

ID3D11Device* Graphics::GetDevice() const
{
	return this->device.Get();
}

bool Graphics::InitializeDirectX(HWND hWnd)
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = displayWidth;
	swapDesc.BufferDesc.Height = displayHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
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
		&swapChain,
		&device,
		nullptr,
		&context
	);

	if (FAILED(res))
	{
		std::cout << "Failed to create device and swapchain" << std::endl;
		return false;
	}

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	if (FAILED(res))
	{
		std::cout << "Failed to create back buffer" << std::endl;
		return false;
	}

	res = device->CreateRenderTargetView(backBuffer, nullptr, &renderView);

	if (FAILED(res))
	{
		std::cout << "Failed to create RenderTargetView" << std::endl;
		return false;
	}
    return true;
}

bool Graphics::InitializeResources()
{
	HRESULT res;

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device->CreateRasterizerState(&rastDesc, &rastState);

	if (FAILED(res))
	{
		std::cout << "Failed to create rasterization state" << std::endl;
		return false;
	}

	context->RSSetState(rastState);

	//Create depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = displayWidth;
	depthStencilDesc.Height = displayHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	res = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);

	if (FAILED(res))
	{
		std::cout << "Error to create depthStencilBuffer texture" << std::endl;
	}

	res = device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);

	if (FAILED(res))
	{
		std::cout << "Error to create depthStencilView" << std::endl;
	}

	//Create shaders
	for (int i = 0; i < vertexShaders.size(); i++)
	{
		if (!vertexShaders[i]->Initialize(device.Get()))
		{
			return false;
		}
	}

	for (int i = 0; i < pixelShaders.size(); i++)
	{
		if (!pixelShaders[i]->Initialize(device.Get()))
		{
			return false;
		}
	}

	//Create textures
	for (int i = 0; i < textures.size(); i++)
	{
		if (FAILED(textures[i]->Initialize(device.Get())))
		{
			std::cout << "Texture " << i << " init failed" << std::endl;
			return false;
		}
	}
	return true;
}
