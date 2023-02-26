#include "PlanetSystemComponent.h"
#include "Game.h"
#include <random>

void PlanetSystemComponent::FollowPlanet()
{
	Vector3 pos = followTarget->Transform.GetPosition();

	float offsetScale = 0.7f;
	Vector3 offset = followTarget->Transform.GetScale() * -2.0f;
	offset = Vector3(offset.x * -offsetScale, offset.y * -offsetScale, offset.z * offsetScale);

	pos += offset;

	game->MainCamera->Transform.SetPosition(pos);
}

PlanetSystemComponent::PlanetSystemComponent(Game* game, Transform3D transform, PlanetComponent* star, std::vector<PlanetComponent*> planets)
	: GameComponent(game, transform)
{
	this->planets = planets;
	this->star = star;
	this->isFollowMode = false;
	this->followTarget = nullptr;
	this->SimulationSpeed = 1.0f;
}

void PlanetSystemComponent::Update()
{
	GameComponent::Update();

	for (int i = 0; i < planets.size(); i++)
	{
		RotatePlanet(planets[i]);
	}

	if (isFollowMode) 
	{
		FollowPlanet();
		if (game->Input->IsKeyDown(Keys::W)
			|| game->Input->IsKeyDown(Keys::A)
			|| game->Input->IsKeyDown(Keys::S)
			|| game->Input->IsKeyDown(Keys::D))
		{
			isFollowMode = false;
		}
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

	Vector3 angle = Vector3(0.0f, game->DeltaTime * orbSpeed * SimulationSpeed, 0.0f);

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

void PlanetSystemComponent::StartFollowPlanet(PlanetComponent* p)
{
	followTarget = p;
	isFollowMode = true;

	Vector3 rot = Vector3(45.0f, -45.0f, 0.0f);
	game->MainCamera->Transform.SetRotation(rot);
	FollowPlanet();
}
