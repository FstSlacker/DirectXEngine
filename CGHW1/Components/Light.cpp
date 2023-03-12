#include "Light.h"
#include "../Game.h"

DirectX::SimpleMath::Color Light::AmbientColor = DirectX::SimpleMath::Color(DirectX::Colors::White);
float Light::AmbientIntensity = 0.1f;

void Light::AddLightComponent(LightComponent* lightComp)
{
	lights.push_back(lightComp);
}

LightComponent* Light::GetLightComponent(int ind) const
{
	return lights[ind];
}

Light::Light(Game* game)
{
	this->game = game;
}

int Light::GetLightSourcesCount() const
{
	return lights.size();
}

bool Light::Initialize()
{
	HRESULT res = lightsBuffer.Initialize(game->Gfx.GetDevice());
	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to init lightsBuffer");
		return false;
	}

	lightsBuffer.SetSlot(1);

	return true;
}

void Light::Bind()
{
	lightsBuffer.Data.AmbientColor = Light::AmbientColor.ToVector3(); // 12
	lightsBuffer.Data.AmbientIntensity = Light::AmbientIntensity; // 4
	lightsBuffer.Data.CameraPosition = game->MainCamera->Transform.GetPosition(); // 12

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		LightData lightData = {};
		lightData.Enabled = false;

		if (i < lights.size() && lights[i]->IsEnabled)
		{
			LightComponent* light = lights[i];

			lightData.Position = DirectX::SimpleMath::Vector4(light->Transform.GetPosition()); 
			lightData.Color = light->LightColor; 
			lightData.Intensity = light->Intensity;
			lightData.Enabled = true;

			if (typeid(*light) == typeid(PointLightComponent))
			{
				PointLightComponent* pLight = dynamic_cast<PointLightComponent*>(light);
				lightData.Range = pLight->Range;
				lightData.ConstantAtt = pLight->ConstantAttenuation;
				lightData.LinearAtt = pLight->LinearAttenuation;
				lightData.QuadraticAtt = pLight->QuadricAttenuation;
				lightData.LightType = (int)LightType::Point;
			}
			else if (typeid(*light) == typeid(DirectionalLightComponent))
			{
				lightData.Direction = Vector4(lights[i]->Transform.GetForward());
				lightData.LightType = (int)LightType::Directional;
			}
			else if (typeid(*light) == typeid(SpotLightComponent))
			{
				SpotLightComponent* spLight = dynamic_cast<SpotLightComponent*>(light);
				lightData.Range = spLight->Range;
				lightData.ConstantAtt = spLight->ConstantAttenuation;
				lightData.LinearAtt = spLight->LinearAttenuation;
				lightData.QuadraticAtt = spLight->QuadricAttenuation;
				lightData.Direction = Vector4(lights[i]->Transform.GetForward());
				lightData.SpotAngle = dynamic_cast<SpotLightComponent*>(light)->ConeAngle * 0.01745329f;
				lightData.LightType = (int)LightType::Spot;
			}
		}

		lightsBuffer.Data.Lights[i] = lightData;
			
	}

	HRESULT hr = lightsBuffer.ApplyChanges(game->Gfx.GetContext());
	
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to apply lightsBuffer");
	}

	lightsBuffer.Bind(game->Gfx.GetContext());
}

LightComponent::LightComponent(Game* game) : GameComponent(game)
{
	this->LightColor = Color(DirectX::Colors::White);
	this->Intensity = 1.0f;
}

PointLightComponent::PointLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "PointLight_" + std::to_string(game->Light.GetLightSourcesCount());
	this->Range = 10.0f;
	this->ConstantAttenuation = 0.0f;
	this->LinearAttenuation = 1.0f;
	this->QuadricAttenuation = 0.0f;
	game->Light.AddLightComponent(this);
}

DirectionalLightComponent::DirectionalLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "DirectionalLight_" + std::to_string(game->Light.GetLightSourcesCount());
	game->Light.AddLightComponent(this);
}

SpotLightComponent::SpotLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "SpotLight_" + std::to_string(game->Light.GetLightSourcesCount());
	this->ConeAngle = 30.0f;
	this->Range = 10.0f;
	this->ConstantAttenuation = 0.0f;
	this->LinearAttenuation = 1.0f;
	this->QuadricAttenuation = 0.0f;
	game->Light.AddLightComponent(this);
}
