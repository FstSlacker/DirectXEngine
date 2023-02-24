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

	return res;
}

void Texture::Bind(ID3D11DeviceContext* context)
{
	context->PSSetShaderResources(0, 1, textureView.GetAddressOf());
}

void Texture::DestroyResources()
{
	textureView->Release();
}
