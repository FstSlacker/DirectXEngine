#include "Material.h"
#include <DirectXColors.h>

Material::Material(VertexShader* vs, PixelShader* ps)
{
	this->EmissiveColor = Color(DirectX::Colors::Black);
	this->AmbientColor = Color(DirectX::Colors::White);
	this->DiffuseColor = Color(DirectX::Colors::White);
	this->SpecularColor = Color(DirectX::Colors::White);
	this->SpecularPower = 16;
	this->DiffuseTexture = nullptr;

	this->vertexShader = vs;
	this->pixelShader = ps;
}

bool Material::Initialize(ID3D11Device* device)
{
	HRESULT hr;
	hr = materialBuffer.Initialize(device);
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize materialBuffer");
		return false;
	}

	return true;
}

void Material::Bind(ID3D11DeviceContext* context)
{
	materialBuffer.Data.Emissive = EmissiveColor;
	materialBuffer.Data.Ambient = AmbientColor;
	materialBuffer.Data.Diffuse = DiffuseColor;
	materialBuffer.Data.Specular = SpecularColor;
	materialBuffer.Data.SpecularPower = SpecularPower;

	if (DiffuseTexture != nullptr)
	{
		materialBuffer.Data.UseTexture = true;
		DiffuseTexture->Bind(context);
	}
	else
	{
		materialBuffer.Data.UseTexture = false;
	}

	HRESULT hr = materialBuffer.ApplyChanges(context);
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to apply materialBuffer");
	}

	materialBuffer.Bind(context);
	vertexShader->Bind(context);
	pixelShader->Bind(context);
}

void Material::DestroyResources()
{
	//...
}
