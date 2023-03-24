#include "Graphics.h"
#include "../Logs.h"

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

	isInitialized = true;

    return true;
}

bool Graphics::Resize(UINT width, UINT height)
{
	if (!isInitialized)
		return false;

	this->displayWidth = width;
	this->displayHeight = height;

	HRESULT res;

	context->OMSetRenderTargets(0, 0, 0);

	renderView->Release();
	depthStencil.DestroyResources();
	backBuffer->Release();

	res = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to resize Swapchain");
		return false;
	}

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create BackBuffer");
		return false;
	}

	if (!depthStencil.Initialize(device.Get(), this->displayWidth, this->displayHeight))
	{
		return false;
	}

	res = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderView.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create RenderTargetView");
		return false;
	}
	return true;
}

void Graphics::DestroyResources()
{
	swapChain.Reset();
	device.Reset();
	context.Reset();
	backBuffer.Reset();
	renderView.Reset();
}

void Graphics::PrepareFrame()
{
	context->ClearState();

	context->RSSetState(rastState.Get());

	SetDepthStencilEnable(true);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(displayWidth);
	viewport.Height = static_cast<float>(displayHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	context->OMSetRenderTargets(1, renderView.GetAddressOf(), depthStencil.GetView());

	float color[] = { BackgroundColor.x, BackgroundColor.y, BackgroundColor.z, 1.0f };
	context->ClearRenderTargetView(renderView.Get(), color);
	depthStencil.Clear(context.Get());
}

void Graphics::BindRenderTarget(ID3D11DepthStencilView* depthStencilView)
{
	context->OMSetRenderTargets(1, renderView.GetAddressOf(), depthStencilView);
}

void Graphics::BindRenderTarget()
{
	context->OMSetRenderTargets(1, renderView.GetAddressOf(), depthStencil.GetView());
}

void Graphics::EndFrame()
{
	//context->OMSetRenderTargets(0, nullptr, nullptr);
	swapChain->Present(1, 0);
}

void Graphics::SetDepthStencilEnable(bool isEnable)
{
	if (isEnable)
	{
		context->OMSetDepthStencilState(depthStencilStateEnabled.Get(), 0);
	}
	else
	{
		context->OMSetDepthStencilState(depthStencilStateDisabled.Get(), 0);
	}
}

ID3D11DeviceContext* Graphics::GetContext() const
{
	return this->context.Get();
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
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		context.GetAddressOf()
	);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create Device and Swapchain");
		return false;
	}

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create BackBuffer");
		return false;
	}

	res = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderView.GetAddressOf());

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
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = device->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create RasterizationState");
		return false;
	}

	context->RSSetState(rastState.Get());

	//Create depth stencil view
	if (!depthStencil.Initialize(device.Get(), displayWidth, displayHeight))
	{
		return false;
	}

	//Create depth stencil states
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	res = device->CreateDepthStencilState(&depthStencilStateDesc, depthStencilStateEnabled.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilStateEnabled");
		return false;
	}

	depthStencilStateDesc.DepthEnable = false;

	res = device->CreateDepthStencilState(&depthStencilStateDesc, depthStencilStateDisabled.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilStateDisabled");
		return false;
	}

	return true;
}
