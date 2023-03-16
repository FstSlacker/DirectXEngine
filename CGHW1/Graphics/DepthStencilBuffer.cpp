#include "DepthStencilBuffer.h"
#include "../Logs.h"

bool DepthStencilBuffer::Initialize(ID3D11Device* device, UINT width, UINT height)
{
	//Create depth stencil view
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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

	res = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create DepthStencilView");
		return false;
	}

	return true;
}

void DepthStencilBuffer::Bind(ID3D11DeviceContext* context)
{

}

void DepthStencilBuffer::Clear(ID3D11DeviceContext* context)
{
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void DepthStencilBuffer::DestroyResources()
{
	depthStencilBuffer.Reset();
	depthStencilView.Reset();
}

ID3D11Texture2D* DepthStencilBuffer::GetBuffer() const
{
	return this->depthStencilBuffer.Get();
}

ID3D11DepthStencilView* DepthStencilBuffer::GetView() const
{
	return this->depthStencilView.Get();
}
