#include "Light.h"
#include "../Game.h"
#include "../Graphics/CascadeShadowMapPass.h"
#include "../Graphics/RenderSystem.h"

//DirectX::SimpleMath::Color Light::AmbientColor = DirectX::SimpleMath::Color(DirectX::Colors::White);
//float Light::AmbientIntensity = 0.1f;
//
//void Light::AddLightComponent(LightComponent* lightComp)
//{
//	lights.push_back(lightComp);
//}
//
//LightComponent* Light::GetLightComponent(int ind) const
//{
//	return lights[ind];
//}
//
//Light::Light(Game* game)
//{
//	this->game = game;
//}
//
//int Light::GetLightSourcesCount() const
//{
//	return lights.size();
//}
//
//bool Light::Initialize()
//{
//	HRESULT res;
//
//	res = lightsBuffer.Initialize(game->Gfx.GetDevice());
//
//	if (FAILED(res))
//	{
//		Logs::LogError(res, "Failed to init lightsBuffer");
//		return false;
//	}
//
//	lightsBuffer.SetSlot(1);
//
//	res = lightsVPBuffer.Initialize(game->Gfx.GetDevice());
//
//	if (FAILED(res))
//	{
//		Logs::LogError(res, "Failed to init lightsVPBuffer");
//		return false;
//	}
//
//	lightsVPBuffer.SetSlot(3);
//
//	res = shadowSampler.Initialize(game->Gfx.GetDevice());
//
//	if (FAILED(res))
//	{
//		Logs::LogError(res, "Failed to init shadowSampler");
//		return false;
//	}
//
//	shadowSampler.SetSlot(1);
//
//	return true;
//}
//
//void Light::Bind(ID3D11DeviceContext* context)
//{
//	lightsBuffer.Data.AmbientColor = Light::AmbientColor.ToVector3(); // 12
//	lightsBuffer.Data.AmbientIntensity = Light::AmbientIntensity; // 4
//	lightsBuffer.Data.CameraPosition = Camera::Main->Transform.GetPosition(); // 12
//
//	int lightIndex = 1;
//
//	for (int i = 0; i < MAX_LIGHTS; i++)
//	{
//		LightData lightData = {};
//		lightData.Enabled = false;
//
//		if (i < lights.size() && lights[i]->IsEnabled())
//		{
//			LightComponent* light = lights[i];
//
//			lightData.Position = DirectX::SimpleMath::Vector4(light->Transform.GetPosition()); 
//			lightData.Color = light->LightColor; 
//			lightData.Intensity = light->Intensity;
//			lightData.Enabled = true;
//
//			if (typeid(*light) == typeid(DirectionalLightComponent))
//			{
//				lightData.Direction = Vector4(lights[i]->Transform.GetForward());
//				lightData.LightType = (int)LightType::Directional;
//
//				//Set light data
//				lightsBuffer.Data.Lights[0] = lightData;
//
//				//Set shadow data
//				float dists[4] = { 0.0f, 25.0f, 90.0f, 200.0f };
//
//				UINT mapsCount = light->GetRenderCameras().size();
//				for (UINT j = 0; j < mapsCount; j++)
//				{
//					lightsVPBuffer.Data.DirectionalLightVP[j] = DirectX::XMMatrixTranspose(light->GetRenderCameras()[j]->GetViewProjectionMatrix());
//					lightsVPBuffer.Data.CascadeDistances[j].z = dists[j + 1];
//					light->GetRenderTargets()[j]->BindAsTexture(context, j + 3u);
//				}
//			}
//			else
//			{
//				if (lightIndex >= MAX_LIGHTS)
//					continue;
//
//				if (typeid(*light) == typeid(PointLightComponent))
//				{
//					PointLightComponent* pLight = dynamic_cast<PointLightComponent*>(light);
//					lightData.Range = pLight->Range;
//					lightData.ConstantAtt = pLight->ConstantAttenuation;
//					lightData.LinearAtt = pLight->LinearAttenuation;
//					lightData.QuadraticAtt = pLight->QuadricAttenuation;
//					lightData.LightType = (int)LightType::Point;
//				}
//				else if (typeid(*light) == typeid(SpotLightComponent))
//				{
//					SpotLightComponent* spLight = dynamic_cast<SpotLightComponent*>(light);
//					lightData.Range = spLight->Range;
//					lightData.ConstantAtt = spLight->ConstantAttenuation;
//					lightData.LinearAtt = spLight->LinearAttenuation;
//					lightData.QuadraticAtt = spLight->QuadricAttenuation;
//					lightData.Direction = Vector4(lights[i]->Transform.GetForward());
//					lightData.SpotAngle = dynamic_cast<SpotLightComponent*>(light)->ConeAngle * 0.01745329f;
//					lightData.LightType = (int)LightType::Spot;
//				}
//
//				//Set light data
//				lightsBuffer.Data.Lights[lightIndex] = lightData;
//
//				//Set shadow data
//				lightsVPBuffer.Data.LightsVP[lightIndex - 1] = DirectX::XMMatrixTranspose(light->GetRenderCameras()[0]->GetViewProjectionMatrix());
//				light->GetRenderTargets()[0]->BindAsTexture(context, (lightIndex - 1) + 6u);
//
//				lightIndex++;
//			}
//		}
//		else
//		{
//			lightsBuffer.Data.Lights[lightIndex] = lightData;
//		}
//
//		
//	}
//
//	HRESULT hr;
//
//	hr = lightsBuffer.ApplyChanges(context);
//	
//	if (FAILED(hr))
//	{
//		Logs::LogError(hr, "Failed to apply lightsBuffer");
//	}
//
//	hr = lightsVPBuffer.ApplyChanges(context);
//
//	if (FAILED(hr))
//	{
//		Logs::LogError(hr, "Failed to apply lightsVPBuffer");
//	}
//
//	//Bind light data
//	lightsBuffer.Bind(context);
//
//	//Bind shadow data
//	lightsVPBuffer.Bind(context);
//	shadowSampler.Bind(context);
//}
//
//void Light::DestroyResources()
//{
//	lightsBuffer.DestroyResources();
//	lightsVPBuffer.DestroyResources();
//	shadowSampler.DestroyResources();
//}

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
	this->Name = "PointLight";
	this->Range = 10.0f;
	this->ConstantAttenuation = 0.0f;
	this->LinearAttenuation = 1.0f;
	this->QuadricAttenuation = 0.0f;
}

LightType PointLightComponent::GetLightType() const
{
	return LightType::Point;
}

PixelShaderLight PointLightComponent::GetPixelShaderLightType() const
{
	return PixelShaderLight::PointLight;
}

LightData PointLightComponent::GetLightData() const
{
	LightData data = {};
	data.Position = Vector4(Transform.GetPosition());
	data.Params = Vector4(Range, ConstantAttenuation, LinearAttenuation, QuadricAttenuation);
	data.Color = Vector4(LightColor.x, LightColor.y, LightColor.z, Intensity);

	return data;
}

void PointLightComponent::DrawGizmos()
{
	LightComponent::DrawGizmos();
	game->Gizmos.DrawSphere(this->Transform.GetPosition(), this->Range, Color(DirectX::Colors::White));
}

DirectionalLightComponent::DirectionalLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "DirectionalLight";
}

LightType DirectionalLightComponent::GetLightType() const
{
	return LightType::Directional;
}

PixelShaderLight DirectionalLightComponent::GetPixelShaderLightType() const
{
	return PixelShaderLight::DirectionalLight;
}

LightData DirectionalLightComponent::GetLightData() const
{
	LightData data = {};
	data.Position = Vector4(Transform.GetPosition());
	data.Direction = Vector4(Transform.GetForward());
	data.Color = Vector4(LightColor.x, LightColor.y, LightColor.z, Intensity);

	return data;
}

void DirectionalLightComponent::DrawGizmos()
{
	LightComponent::DrawGizmos();

	Vector3 center = this->Transform.GetPosition();
	float pScale = (center - Camera::Main->Transform.GetPosition()).Length() * 0.04f;

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
	this->Name = "SpotLight";
	this->ConeAngle = 30.0f;
	this->Range = 10.0f;
	this->ConstantAttenuation = 0.0f;
	this->LinearAttenuation = 1.0f;
	this->QuadricAttenuation = 0.0f;
}

LightType SpotLightComponent::GetLightType() const
{
	return LightType::Spot;
}

PixelShaderLight SpotLightComponent::GetPixelShaderLightType() const
{
	return PixelShaderLight::SpotLight;
}

LightData SpotLightComponent::GetLightData() const
{
	LightData data = {};
	data.Position = Vector4(Transform.GetPosition());
	data.Direction = Vector4(Transform.GetForward());
	data.Direction.w = ConeAngle;
	data.Params = Vector4(Range, ConstantAttenuation, LinearAttenuation, QuadricAttenuation);
	data.Color = Vector4(LightColor.x, LightColor.y, LightColor.z, Intensity);

	return data;
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

AmbientLightComponent::AmbientLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "AmbientLight";
}

LightType AmbientLightComponent::GetLightType() const
{
	return LightType::Ambient;
}

PixelShaderLight AmbientLightComponent::GetPixelShaderLightType() const
{
	return PixelShaderLight::AmbientLight;
}

LightData AmbientLightComponent::GetLightData() const
{
	LightData data = {};
	data.Color = Vector4(LightColor.x, LightColor.y, LightColor.z, Intensity);

	return data;
}

void AmbientLightComponent::DrawGizmos()
{
	//...
}

void AmbientLightComponent::DrawGizmosIcon(Vector3 right, Vector3 up, float scale)
{
	//...
}
