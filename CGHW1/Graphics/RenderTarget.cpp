#include "RenderTarget.h"

bool RenderTarget::Initialize(ID3D11Device* device, UINT width, UINT height)
{

	HRESULT result;


	// Setup the render target texture description.
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, texture.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(texture.Get(), &renderTargetViewDesc, targetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, textureView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

    return true;
}

void RenderTarget::BindAsTexture(ID3D11DeviceContext* context, UINT slot)
{
    context->PSSetShaderResources(slot, 1, textureView.GetAddressOf());
}

void RenderTarget::BindAsTarget(ID3D11DeviceContext* context)
{
    context->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);
}

void RenderTarget::BindAsTarget(ID3D11DeviceContext* context, DepthStencil& depthStencil)
{
    context->OMSetRenderTargets(1, targetView.GetAddressOf(), depthStencil.GetView());
}

void RenderTarget::Clear(ID3D11DeviceContext* context, DirectX::SimpleMath::Color c)
{
	float color[4] = { c.x, c.y, c.z, c.w };
	context->ClearRenderTargetView(targetView.Get(), color);
}

ID3D11ShaderResourceView* RenderTarget::GetShaderResourceView() const
{
	return this->textureView.Get();
}

ID3D11RenderTargetView* RenderTarget::GetRenderTargetView() const
{
	return this->targetView.Get();
}

void RenderTarget::DestroyResources()
{
    targetView.Reset();
    textureView.Reset();
}
