#include "PlanetComponent.h"
#include "Game.h"

PlanetComponent::PlanetComponent(Game* game, Vector3 position, PlanetInfo info, Texture tex)
	: GameComponent(game, Transform3D(position, Vector3::Zero, Vector3(info.Diameter, info.Diameter, info.Diameter)))
{
	this->planetTexture = tex;
	this->Info = info;
	this->IsStar = false;
	this->Name = info.Name;
}

void PlanetComponent::ReloadGraphics(bool isLighting)
{
	effect = std::make_unique<BasicEffect>(game->Device.Get());
	effect->SetTextureEnabled(true);

	effect->SetPerPixelLighting(isLighting);
	effect->SetLightingEnabled(isLighting);
	effect->SetLightEnabled(0, isLighting);

	effect->SetLightDiffuseColor(0, Colors::White);
	effect->SetLightDirection(0, -Vector3::UnitZ);

	sphere = GeometricPrimitive::CreateSphere(game->Context, 1.0f, 16U, false);
	sphere->CreateInputLayout(effect.get(), inputLayout.ReleaseAndGetAddressOf());

	effect->SetTexture(planetTexture.GetTextureView());
}

void PlanetComponent::SetLighting(bool isActive)
{
	ReloadGraphics(isActive);
}

void PlanetComponent::Update()
{
	GameComponent::Update();
}

void PlanetComponent::Draw()
{
	effect->SetView(game->MainCamera->GetViewMatrix());
	effect->SetProjection(game->MainCamera->GetProjectionMatrix());
	effect->SetWorld(Transform.GetTransformMatrix());

	sphere->Draw(effect.get(), inputLayout.Get());
}

void PlanetComponent::Initialize()
{
	planetTexture.Initialize(game->Device.Get());

	ReloadGraphics(!IsStar);

	effect->SetTexture(planetTexture.GetTextureView());

}

void PlanetComponent::SetLightDirection(Vector3 dir)
{
	effect->SetLightDirection(0, dir);
}
