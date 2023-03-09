#include "PointLightComponent.h"
#include "Game.h"

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
	HRESULT res = psLightBuffer.Initialize(game->Gfx.GetDevice());
	if (FAILED(res))
	{
		std::cout << "Failed to create psLightBuffer" << std::endl;
	}

}

void PointLightComponent::Bind()
{
	psLightBuffer.Data.LightPosition = this->Transform.GetPosition();
	psLightBuffer.Data.LightStrength = this->LightStrength;
	psLightBuffer.Data.LightColor = this->LightColor.ToVector3();
	psLightBuffer.Data.LightRange = this->LightRange;

	if (!psLightBuffer.ApplyChanges(game->Gfx.GetContext()))
	{
		std::cout << "Failed to apply changes in psLightBuffer" << std::endl;
	}

	psLightBuffer.Bind(game->Gfx.GetContext());
}

void PointLightComponent::Draw()
{
	
}
