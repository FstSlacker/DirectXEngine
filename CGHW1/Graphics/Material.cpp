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

MaterialCBuf Material::GetMaterialData() const
{
	MaterialCBuf data = {};

	data.Emissive = EmissiveColor;
	data.Ambient = AmbientColor;
	data.Diffuse = DiffuseColor;
	data.Specular = XMFLOAT4(SpecularColor.x, SpecularColor.y, SpecularColor.z, SpecularPower);

	data.Flags = 0;

	if (DiffuseTexture != nullptr)
		data.Flags |= USE_DIFFUSE_TEXTURE;

	if (NormalMapTexture != nullptr)
		data.Flags |= USE_NORMAL_MAP;

	if (SpecularMapTexture != nullptr)
		data.Flags |= USE_SPECULAR_MAP;

	return data;
}

