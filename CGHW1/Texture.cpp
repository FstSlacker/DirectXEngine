#include "Texture.h"

Texture::Texture()
{
	textureData.Type = TextureType::ColorArray;
	textureData.TextureColor = Color(1.0f, 0.0f, 0.0f);
}

Texture::Texture(Color color)
{
	textureData.Type = TextureType::ColorArray;
	textureData.TextureColor = color;
}

Texture::Texture(std::wstring imagePath)
{
	textureData.Type = TextureType::FilePath;
	textureData.TexturePath = imagePath;
}

HRESULT Texture::Initialize(ID3D11Device* device)
{
	HRESULT res = sampler.Initialize(device);

	if (FAILED(res))
	{
		return res;
	}
	if (textureData.Type == TextureType::FilePath)
	{
		return InitializeFromFile(device, textureData.TexturePath);
	}
	else if (textureData.Type == TextureType::ColorArray)
	{
		Color* colorData = new Color[1]{ textureData.TextureColor };
		res = InitializeFromColor(device, colorData, 1, 1);
		delete[] colorData;
		return res;
	}

	return 0;
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

HRESULT Texture::InitializeFromColor(ID3D11Device* device, const Color* colorData, UINT w, UINT h)
{
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R32G32B32A32_FLOAT, w, h);
	ID3D11Texture2D* tex2d = nullptr;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = colorData;
	initData.SysMemPitch = w * sizeof(Color);
	HRESULT res = device->CreateTexture2D(&textureDesc, &initData, &tex2d);

	if (FAILED(res))
	{
		return res;
	}

	texture = static_cast<ID3D11Texture2D*>(tex2d);

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	res = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());

	return res;
}

HRESULT Texture::InitializeFromFile(ID3D11Device* device, std::wstring path)
{
	HRESULT res = DirectX::CreateWICTextureFromFile(
		device,
		path.c_str(),
		nullptr,
		textureView.GetAddressOf()
	);

	return res;
}
