#include "Texture.h"

Texture::Texture(std::wstring imagePath)
{
	this->texPath = imagePath;
}

HRESULT Texture::Initialize(ID3D11Device* device)
{
	HRESULT res = DirectX::CreateWICTextureFromFile(
		device,
		texPath.c_str(),
		nullptr,
		textureView.GetAddressOf()
	);

	if (FAILED(res))
		return res;

	res = sampler.Initialize(device);

	return res;
}

ID3D11ShaderResourceView* Texture::GetTextureView() const
{
	return this->textureView.Get();
}

void Texture::Bind(ID3D11DeviceContext* context)
{
	sampler.Bind(context);
	context->PSSetShaderResources(0, 1, textureView.GetAddressOf());
}

void Texture::DestroyResources()
{
	textureView->Release();
	sampler.DestroyResources();
}
