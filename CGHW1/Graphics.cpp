#include "Graphics.h"
#include "Logs.h"

std::wstring Graphics::kShaderFolder = L"./Shaders/";

bool Graphics::Initialize(HWND hWnd, UINT width, UINT height)
{
	this->displayWidth = width;
	this->displayHeight = height;

    if (!InitializeDirectX(hWnd))
    {
		Logs::Log("Failed to init directx", false);
        return false;
    }
	if (!InitializeResources())
	{
		Logs::Log("Failed to init graphics resources", false);
		return false;
	}

    return true;
}

void Graphics::DestroyResources()
{
	std::map<std::wstring, std::unique_ptr<VertexShader>>::iterator itVs;
	for (itVs = vertexShaders.begin(); itVs != vertexShaders.end(); itVs++)
	{
		itVs->second->DestroyResources();
	}

	std::map<std::wstring, std::unique_ptr<PixelShader>>::iterator itPs;
	for (itPs = pixelShaders.begin(); itPs != pixelShaders.end(); itPs++)
	{
		itPs->second->DestroyResources();
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
	SetDepthStencilEnable(true);

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

void Graphics::AddVertexShader(std::wstring name)
{
	vertexShaders[name] = std::make_unique<VertexShader>(kShaderFolder + name, VertexShader::VertexLayoutType::VectrxPositionColorTextureNormal);
}

void Graphics::AddPixelShader(std::wstring name)
{
	pixelShaders[name] = std::make_unique<PixelShader>(kShaderFolder + name);
}

VertexShader* Graphics::FindVertexShader(std::wstring name) const
{
	if (vertexShaders.count(name))
	{
		return vertexShaders.at(name).get();
	}

	return nullptr;
}

PixelShader* Graphics::FindPixelShader(std::wstring name) const
{
	if (pixelShaders.count(name))
	{
		return pixelShaders.at(name).get();
	}

	return nullptr;
}

void Graphics::AddTexture(Texture* tex)
{
	textures.push_back(tex);
}

void Graphics::AddMaterial(Material* mat)
{
	this->materials.push_back(mat);
}

void Graphics::SetDepthStencilEnable(bool isEnable)
{
	if (isEnable)
	{
		context->OMSetDepthStencilState(depthStencilStateEnabled, 0);
	}
	else
	{
		context->OMSetDepthStencilState(depthStencilStateDisabled, 0);
	}
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
		Logs::LogError(res, "Failed to create Device and Swapchain");
		return false;
	}

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create BackBuffer");
		return false;
	}

	res = device->CreateRenderTargetView(backBuffer, nullptr, &renderView);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create RenderTargetView");
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
		Logs::LogError(res, "Failed to create RasterizationState");
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
		Logs::LogError(res, "Failed to create DepthStencilBuffer texture");
		return false;
	}

	res = device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilView");
		return false;
	}

	//Create depth stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	res = device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilStateEnabled);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilStateEnabled");
		return false;
	}

	depthStencilStateDesc.DepthEnable = false;

	res = device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilStateDisabled);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilStateDisabled");
		return false;
	}

	//Init shaders

	std::map<std::wstring, std::unique_ptr<VertexShader>>::iterator itVs;
	for (itVs = vertexShaders.begin(); itVs != vertexShaders.end(); itVs++)
	{
		if (!itVs->second->Initialize(device.Get()))
		{
			return false;
		}
	}

	std::map<std::wstring, std::unique_ptr<PixelShader>>::iterator itPs;
	for (itPs = pixelShaders.begin(); itPs != pixelShaders.end(); itPs++)
	{
		if (!itPs->second->Initialize(device.Get()))
		{
			return false;
		}
	}

	//Init textures
	for (int i = 0; i < textures.size(); i++)
	{
		if (!textures[i]->Initialize(device.Get()))
		{
			return false;
		}
	}

	//Init materials
	for (int i = 0; i < materials.size(); i++)
	{
		if (!materials[i]->Initialize(device.Get()))
		{
			return false;
		}
	}

	return true;
}
