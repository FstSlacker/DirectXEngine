#include "DepthStencil.h"
#include "../Logs.h"

DXGI_FORMAT DepthStencil::GetUsageTypelessFormat(UsageFormat format)
{
	switch (format)
	{
		case UsageFormat::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		case UsageFormat::ShadowDepth:
			return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	}
	throw std::runtime_error("Undefined format");
}

DXGI_FORMAT DepthStencil::GetUsageTypedFormat(UsageFormat format)
{
	switch (format)
	{
	case UsageFormat::DepthStencil:
		return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	case UsageFormat::ShadowDepth:
		return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	}
	throw std::runtime_error("Undefined format");
}

DXGI_FORMAT DepthStencil::GetUsageColoredFormat(UsageFormat format)
{
	switch (format)
	{
	case UsageFormat::DepthStencil:
		return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case UsageFormat::ShadowDepth:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	}
	throw std::runtime_error("Undefined format");
}

bool DepthStencil::Initialize(ID3D11Device* device, UINT width, UINT height, UsageFormat format)
{
	//Create depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = GetUsageTypelessFormat(format);// DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT res;

	res = device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilBuffer texture");
		return false;
	}

	CD3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format = GetUsageTypedFormat(format);
	viewDesc.Flags = 0;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	res = device->CreateDepthStencilView(depthStencilBuffer.Get(), &viewDesc, depthStencilView.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilView");
		return false;
	}

	return true;
}

void DepthStencil::BindAsDepthStencil(ID3D11DeviceContext* context)
{
	context->OMSetRenderTargets(0, nullptr, this->depthStencilView.Get());
}

void DepthStencil::Clear(ID3D11DeviceContext* context)
{
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void DepthStencil::DestroyResources()
{
	depthStencilBuffer.Reset();
	depthStencilView.Reset();
}

ID3D11Texture2D* DepthStencil::GetBuffer() const
{
	return this->depthStencilBuffer.Get();
}

ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return this->depthStencilView.Get();
}
