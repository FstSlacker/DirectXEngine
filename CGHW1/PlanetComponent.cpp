#include "PlanetComponent.h"
#include "Game.h"

PlanetComponent::PlanetComponent(Game* game, Vector3 position, float radius, float orbitRad, float orbitSpeed, Texture tex)
	: GameComponent(game, Transform3D(position, Vector3::Zero, Vector3(radius, radius, radius)))
{
	this->planetTexture = tex;
	this->OrbitRadius = orbitRad;
	this->OrbitSpeed = orbitSpeed;
	this->IsStar = false;
}

void PlanetComponent::Update()
{
	GameComponent::Update();
	Transform.AddRotation(Vector3(0.0f, game->DeltaTime * 10.0f, 0.0f));
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

	effect = std::make_unique<BasicEffect>(game->Device.Get());
	effect->SetTextureEnabled(true);

	effect->SetPerPixelLighting(!IsStar);
	effect->SetLightingEnabled(!IsStar);
	effect->SetLightEnabled(0, !IsStar);
	
	effect->SetLightDiffuseColor(0, Colors::White);
	effect->SetLightDirection(0, -Vector3::UnitZ);

	sphere = GeometricPrimitive::CreateSphere(game->Context, 1.0f, 16U, false);
	sphere->CreateInputLayout(effect.get(), inputLayout.ReleaseAndGetAddressOf());

	effect->SetTexture(planetTexture.GetTextureView());

}

void PlanetComponent::SetLightDirection(Vector3 dir)
{
	effect->SetLightDirection(0, dir);
}
