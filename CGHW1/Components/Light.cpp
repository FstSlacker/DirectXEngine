#include "Light.h"
#include "../Game.h"
#include "../Graphics/CascadeShadowMapPass.h"
#include "../Graphics/RenderSystem.h"

LightComponent::LightComponent(Game* game) : GameComponent(game)
{
	this->LightColor = Color(DirectX::Colors::White);
	this->Intensity = 1.0f;
}

bool LightComponent::Initialize()
{
	if (!GameComponent::Initialize())
		return false;

	if (mesh != nullptr)
	{
		if (!mesh->Initialize(game->Gfx.GetDevice()))
			return false;
	}
}

Mesh* LightComponent::GetMesh() const
{
	return this->mesh.get();
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

	this->mesh = Mesh::CreateSphereMesh(32U, 32U);
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

void PointLightComponent::DrawGui()
{
	LightComponent::DrawGui();

	if (ImGui::CollapsingHeader("PointLight", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lightColor[3] = { this->LightColor.x, this->LightColor.y, this->LightColor.z };
		if (ImGui::ColorEdit3("Color", lightColor))
		{
			this->LightColor = Color(lightColor[0], lightColor[1], lightColor[2]);
		}
		ImGui::DragFloat("Intensity", &this->Intensity, 0.01f, 0.0f, 10.0f);

		ImGui::DragFloat("Range", &this->Range);

		ImGui::Spacing();

		ImGui::Text("Attenuation:");
		ImGui::DragFloat("Constant", &this->ConstantAttenuation);
		ImGui::DragFloat("Linear", &this->LinearAttenuation);
		ImGui::DragFloat("Quadric", &this->QuadricAttenuation);

		ImGui::Spacing();
	}
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

void DirectionalLightComponent::DrawGui()
{
	LightComponent::DrawGui();

	if (ImGui::CollapsingHeader("DirectionalLight", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lightColor[3] = { this->LightColor.x, this->LightColor.y, this->LightColor.z };
		if (ImGui::ColorEdit3("Color", lightColor))
		{
			this->LightColor = Color(lightColor[0], lightColor[1], lightColor[2]);
		}
		ImGui::DragFloat("Intensity", &this->Intensity, 0.01f, 0.0f, 10.0f);

		ImGui::Spacing();
	}
}

SpotLightComponent::SpotLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "SpotLight";
	this->ConeAngle = 30.0f;
	this->Range = 10.0f;
	this->ConstantAttenuation = 0.0f;
	this->LinearAttenuation = 1.0f;
	this->QuadricAttenuation = 0.0f;

	this->mesh = Mesh::CreateSimpleConeMesh(16U, 1.0f, 1.0f);
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
	data.Direction.w = ConeAngle * kDeg2Rad;
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

void SpotLightComponent::DrawGui()
{
	LightComponent::DrawGui();

	if (ImGui::CollapsingHeader("PointLight", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lightColor[3] = { this->LightColor.x, this->LightColor.y, this->LightColor.z };
		if (ImGui::ColorEdit3("Color", lightColor))
		{
			this->LightColor = Color(lightColor[0], lightColor[1], lightColor[2]);
		}
		ImGui::DragFloat("Intensity", &this->Intensity, 0.01f, 0.0f, 10.0f);

		ImGui::DragFloat("Range", &this->Range);
		ImGui::DragFloat("ConeAngle", &this->ConeAngle);

		ImGui::Spacing();

		ImGui::Text("Attenuation:");
		ImGui::DragFloat("Constant", &this->ConstantAttenuation);
		ImGui::DragFloat("Linear", &this->LinearAttenuation);
		ImGui::DragFloat("Quadric", &this->QuadricAttenuation);

		ImGui::Spacing();
	}
}

AmbientLightComponent::AmbientLightComponent(Game* game) : LightComponent(game)
{
	this->Name = "AmbientLight";
	this->Intensity = 0.2f;
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

void AmbientLightComponent::DrawGui()
{
	LightComponent::DrawGui();

	if (ImGui::CollapsingHeader("AmbientLight", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lightColor[3] = { this->LightColor.x, this->LightColor.y, this->LightColor.z };
		if (ImGui::ColorEdit3("Color", lightColor))
		{
			this->LightColor = Color(lightColor[0], lightColor[1], lightColor[2]);
		}
		ImGui::DragFloat("Intensity", &this->Intensity, 0.01f, 0.0f, 10.0f);

		ImGui::Spacing();
	}
}
