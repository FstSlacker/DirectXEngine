#include "PointLightComponent.h"
#include "Game.h"
#include "Logs.h"

PointLightComponent::PointLightComponent(Game* game) : GameComponent(game)
{
	this->Name = "PointLightComponent";
	this->LightColor = DirectX::Colors::White;
	this->LightStrength = 1.0f;
	this->LightRange = 10.0f;
	this->game->Light = this;
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
	psLightBuffer.Data.LightPosition = this->Transform.GetPosition();
	psLightBuffer.Data.LightStrength = this->LightStrength;
	psLightBuffer.Data.LightColor = this->LightColor.ToVector3();
	psLightBuffer.Data.LightRange = this->LightRange;

	psLightBuffer.ApplyChanges(game->Gfx.GetContext());

	psLightBuffer.Bind(game->Gfx.GetContext());
}

void PointLightComponent::Draw()
{
	//...
}
