#include "Light.h"
#include "Game.h"

DirectX::SimpleMath::Color Light::AmbientColor = DirectX::SimpleMath::Color(DirectX::Colors::White);
float Light::AmbientIntensity = 0.1f;

void Light::AddLightComponent(PointLightComponent* lightComp)
{
	lights.push_back(lightComp);
}

PointLightComponent* Light::GetLightComponent(int ind) const
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

		if (i < lights.size())
		{
			PointLightComponent* light = lights[i];
			lightData.Position = DirectX::SimpleMath::Vector4(light->Transform.GetPosition()); // 16 
			lightData.Direction = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); // 16 
			lightData.Color = light->LightColor; // 16 

			lightData.SpotAngle = 0.0f;
			lightData.ConstantAtt = 1.0f;
			lightData.LinearAtt = 0.08f;
			lightData.QuadraticAtt = 0.0f;

			lightData.LightType = (int)LightType::Point;
			lightData.Enabled = true;
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
