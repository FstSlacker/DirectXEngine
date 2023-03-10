#include "PointLightComponent.h"
#include "Game.h"
#include "Logs.h"
#include "Light.h"

DirectX::SimpleMath::Color PointLightComponent::AmbientColor = DirectX::SimpleMath::Color(DirectX::Colors::White);
float PointLightComponent::AmbientIntensity = 0.1f;

PointLightComponent::PointLightComponent(Game* game) : GameComponent(game)
{
	this->Name = "PointLightComponent_" + std::to_string(game->Light.GetLightSourcesCount());
	this->LightColor = DirectX::Colors::White;
	this->Intensity = 1.0f;
	this->SpecularIntensity = 1.0f;
	this->SpecularPower = 1.0f;
	this->Range = 10.0f;
	game->Light.AddLightComponent(this);
	//this->game->Light = this;
}

void PointLightComponent::Initialize()
{
	HRESULT hr = psLightBuffer.Initialize(game->Gfx.GetDevice());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize psLightBuffer");
	}

}

void PointLightComponent::Bind()
{
	psLightBuffer.Data.AmbientColor = PointLightComponent::AmbientColor.ToVector3();
	psLightBuffer.Data.AmbientIntensity = PointLightComponent::AmbientIntensity;

	psLightBuffer.Data.LightPosition = this->Transform.GetPosition();
	psLightBuffer.Data.DiffuseColor = this->LightColor.ToVector3();
	psLightBuffer.Data.DiffuseIntensity = this->Intensity;

	psLightBuffer.Data.SpecularIntensity = this->SpecularIntensity;
	psLightBuffer.Data.SpecularPower = this->SpecularPower;

	psLightBuffer.Data.CameraPosition = game->MainCamera->Transform.GetPosition();

	psLightBuffer.Data.LightRange = this->Range;

	psLightBuffer.ApplyChanges(game->Gfx.GetContext());

	psLightBuffer.Bind(game->Gfx.GetContext());
}

void PointLightComponent::Draw()
{
	//...
}
