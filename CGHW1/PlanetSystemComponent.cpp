#include "PlanetSystemComponent.h"
#include "Game.h"
#include <random>

PlanetSystemComponent::PlanetSystemComponent(Game* game, Transform3D transform, PlanetComponent* star, std::vector<PlanetComponent*> planets) 
	: GameComponent(game, transform)
{
	this->planets = planets;
	this->star = star;
}

void PlanetSystemComponent::Update()
{
	GameComponent::Update();

	for (int i = 0; i < planets.size(); i++)
	{
		RotatePlanet(planets[i]);
	}

}

void PlanetSystemComponent::Initialize()
{
	GameComponent::Initialize();

	Vector3 sysPos = Transform.GetPosition();
	star->Transform.SetPosition(sysPos);

	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->Transform.SetPosition(
			Vector3(
				sysPos.x + planets[i]->OrbitRadius,
				sysPos.y,
				sysPos.z
			)
		);
	}

}

void PlanetSystemComponent::RotatePlanet(PlanetComponent* p)
{
	float orbSpeed = p->OrbitSpeed;

	Vector3 angle = Vector3(0.0f, game->DeltaTime * orbSpeed, 0.0f);

	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	DirectX::XMVECTOR scale, rot, pos;
	DirectX::XMMatrixDecompose(&scale, &rot, &pos, p->Transform.GetTransformMatrix() * rotMat);


	Vector3 newPos = Vector3(
		DirectX::XMVectorGetX(pos),
		DirectX::XMVectorGetY(pos),
		DirectX::XMVectorGetZ(pos)
	);

	p->Transform.SetPosition(newPos);

	Vector3 lightDir = newPos - star->Transform.GetPosition();
	lightDir.Normalize();

	p->SetLightDirection(lightDir);
}
