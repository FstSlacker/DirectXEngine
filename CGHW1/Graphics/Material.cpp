#include "Material.h"
#include <DirectXColors.h>
#include "../Game.h"

Material::Material(Game* game, VertexShader* vs, PixelShader* ps, UINT passInd)
{
	assert(vs != nullptr && ps != nullptr);

	this->EmissiveColor = Color(DirectX::Colors::Black);
	this->AmbientColor = Color(DirectX::Colors::White);
	this->DiffuseColor = Color(DirectX::Colors::White);
	this->SpecularColor = Color(DirectX::Colors::White);
	this->SpecularPower = 16;

	this->DiffuseTexture = nullptr;
	this->NormalMapTexture = nullptr;
	this->SpecularMapTexture = nullptr;

	this->vertexShader = vs;
	this->pixelShader = ps;

	this->SetQueueIndex(passInd);

	//game->Gfx.AddMaterial(this);
	game->Resources.AddResource<Material>(this, "Material_" + std::to_string(game->Resources.GetCount<Material>()));
}

bool Material::AttachToComponent(GameComponent& comp)
{
	return this->AddComponent(&comp);
}

bool Material::RemoveFromComponent(GameComponent& comp)
{
	return this->RemoveComponent(&comp);
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
	materialBuffer.Data.Specular = XMFLOAT4(SpecularColor.x, SpecularColor.y, SpecularColor.z, SpecularPower);

	materialBuffer.Data.Flags = 0;

	if (DiffuseTexture != nullptr)
	{
		DiffuseTexture->SetSlot(0);
		materialBuffer.Data.Flags |= USE_DIFFUSE_TEXTURE;
		DiffuseTexture->Bind(context);
	}

	if (NormalMapTexture != nullptr)
	{
		NormalMapTexture->SetSlot(1);
		materialBuffer.Data.Flags |= USE_NORMAL_MAP;
		NormalMapTexture->Bind(context);
	}

	if (SpecularMapTexture != nullptr)
	{
		SpecularMapTexture->SetSlot(2);
		materialBuffer.Data.Flags |= USE_SPECULAR_MAP;
		SpecularMapTexture->Bind(context);
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

