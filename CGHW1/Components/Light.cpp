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

		if (i < lights.size() && lights[i]->IsEnabled())
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

void LightComponent::DrawGizmos()
{
	GameComponent::DrawGizmos();
}

void LightComponent::DrawGizmosIcon(Vector3 right, Vector3 up, float scale)
{
	Vector3 center = this->Transform.GetPosition();

	game->Gizmos.DrawRing(
		center + up * 0.2f * scale,
		right * 0.7f * scale,
		up * 0.7f * scale,
		Color(DirectX::Colors::Yellow)
	);

	game->Gizmos.DrawQuad(
		center + (right * 0.25f + -up * 0.9f) * scale,
		center + (-right * 0.25f + -up * 0.9f) * scale,
		center + (-right * 0.35f + -up * 0.4f) * scale,
		center + (right * 0.35f + -up * 0.4f) * scale,
		Color(DirectX::Colors::Yellow)
	);
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

void PointLightComponent::DrawGizmos()
{
	LightComponent::DrawGizmos();

	game->Gizmos.DrawSphere(this->Transform.GetPosition(), this->Range, Color(DirectX::Colors::White));
}

DirectionalLightComponent::DirectionalLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "DirectionalLight_" + std::to_string(game->Light.GetLightSourcesCount());
	game->Light.AddLightComponent(this);
}

void DirectionalLightComponent::DrawGizmos()
{
	LightComponent::DrawGizmos();

	Vector3 center = this->Transform.GetPosition();
	float pScale = (center - game->MainCamera->Transform.GetPosition()).Length() * 0.04f;

	Vector3 o = this->Transform.GetPosition();
	Vector3 lf = this->Transform.GetForward();
	Vector3 lr = this->Transform.GetRight();
	Vector3 lu = this->Transform.GetUp();

	Color color = Color(DirectX::Colors::White);

	game->Gizmos.DrawRay(o + lr * 0.5f * pScale, lf * pScale * 4.0f, color);
	game->Gizmos.DrawRay(o + -lr * 0.5f * pScale, lf * pScale * 4.0f, color);
	game->Gizmos.DrawRay(o + lu * 0.5f * pScale, lf * pScale * 4.0f, color);
	game->Gizmos.DrawRay(o + -lu * 0.5f * pScale, lf * pScale * 4.0f, color);
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

void SpotLightComponent::DrawGizmos()
{
	LightComponent::DrawGizmos();

	float ringRadius = std::tan(this->ConeAngle * kDeg2Rad) * this->Range;

	Vector3 sLightPos = this->Transform.GetPosition();
	Vector3 ringCenter = sLightPos + this->Transform.GetForward() * this->Range;
	Vector3 ringUp = this->Transform.GetUp() * ringRadius;
	Vector3 ringRight = this->Transform.GetRight() * ringRadius;

	Color color = Color(DirectX::Colors::White);

	game->Gizmos.DrawRing(ringCenter, ringUp, ringRight, color);

	Vector3 p1 = ringCenter + ringUp;
	Vector3 p2 = ringCenter + ringRight;
	Vector3 p3 = ringCenter - ringUp;
	Vector3 p4 = ringCenter - ringRight;

	game->Gizmos.DrawRay(sLightPos, p1 - sLightPos, color);
	game->Gizmos.DrawRay(sLightPos, p2 - sLightPos, color);
	game->Gizmos.DrawRay(sLightPos, p3 - sLightPos, color);
	game->Gizmos.DrawRay(sLightPos, p4 - sLightPos, color);
}
