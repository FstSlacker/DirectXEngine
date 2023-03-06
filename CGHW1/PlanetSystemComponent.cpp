#include "PlanetSystemComponent.h"
#include "Game.h"
#include <random>
#include <fstream>

PlanetSystemComponent::PlanetSystemComponent(Game* game, Transform3D transform, PlanetComponent* star, std::vector<PlanetComponent*> planets)
	: GameComponent(game, transform)
{
	this->planets = planets;
	this->star = star;
	this->isFollowMode = false;
	this->followTarget = nullptr;
	this->SimulationSpeed = 1.0f;
	this->orbitScale = 1.0f;
	this->isLighting = true;
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

	ScaleDiametersRelativeEarth();

	ResetSystem();
}

void PlanetSystemComponent::RotatePlanet(PlanetComponent* p)
{
	float orbSpeed = 360.0f / (p->Info.OrbitPeriod * 24.0f);
	float rotSpeed = 360.0f / p->Info.RotationPeriod;

	Vector3 angle = Vector3(0.0f, game->DeltaTime * orbSpeed * SimulationSpeed * kSecPerHour, 0.0f);

	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	DirectX::XMVECTOR scale, rot, pos;
	DirectX::XMMatrixDecompose(&scale, &rot, &pos, p->Transform.GetTransformMatrix() * rotMat);

	Vector3 prevPos = p->Transform.GetPosition();

	Vector3 newPos = Vector3(
		DirectX::XMVectorGetX(pos),
		DirectX::XMVectorGetY(pos),
		DirectX::XMVectorGetZ(pos)
	);

	p->Transform.SetPosition(newPos);
	p->Transform.AddLocalRotation(Vector3(0.0f, rotSpeed * SimulationSpeed * kSecPerHour, 0.0f));

	p->MoveSatelites(newPos - prevPos);
	p->RotateSatelites(game->DeltaTime * SimulationSpeed * kSecPerHour);

	if (isFollowMode && followTarget == p)
	{
		game->MainCamera->Transform.AddPosition(newPos - prevPos);
	}
}

void PlanetSystemComponent::StartFollowPlanet(PlanetComponent* p)
{
	followTarget = p;
	isFollowMode = true;

	Vector3 rot = Vector3(45.0f, -45.0f, 0.0f);
	game->MainCamera->Transform.SetRotation(rot);
	
	Vector3 pos = followTarget->Transform.GetPosition();

	float offsetScale = 0.7f;
	Vector3 offset = followTarget->Transform.GetScale() * -2.0f;
	offset = Vector3(offset.x * -offsetScale, offset.y * -offsetScale, offset.z * offsetScale);

	pos += offset;

	game->MainCamera->Transform.SetPosition(pos);
}

void PlanetSystemComponent::UnfollowPlanet()
{
	this->isFollowMode = false;
	this->followTarget = nullptr;
}

void PlanetSystemComponent::ScaleDiametersRelativeEarth()
{
	float earthDiameter = 12756.0f;

	for (int i = 0; i < planets.size(); i++)
	{
		float d = planets[i]->Info.Diameter / earthDiameter;
		planets[i]->Transform.SetScale(Vector3(d, d, d));
	}
}

void PlanetSystemComponent::SetLighting(bool isActive)
{
	/*if (isLighting == isActive)
		return;

	isLighting = isActive;
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->SetLighting(isActive);
	}*/
}

void PlanetSystemComponent::SetOrbitScale(float orbScale)
{
	this->orbitScale = orbScale;
	for (int i = 0; i < planets.size(); i++)
	{
		Vector3 starPos = star->Transform.GetPosition();
		Vector3 dir = planets[i]->Transform.GetPosition() - starPos;
		dir.Normalize();
		dir = dir * (planets[i]->Info.OrbitRadius * orbScale);

		Vector3 prevPos = planets[i]->Transform.GetPosition();
		planets[i]->Transform.SetPosition(dir + starPos);

		planets[i]->MoveSatelites(planets[i]->Transform.GetPosition() - prevPos);

	}
}

void PlanetSystemComponent::ResetSystem()
{
	Vector3 sysPos = Transform.GetPosition();
	star->Transform.SetPosition(sysPos);

	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->Transform.SetPosition(
			Vector3(
				sysPos.x + planets[i]->Info.OrbitRadius * orbitScale,
				sysPos.y,
				sysPos.z
			)
		);

		planets[i]->Transform.SetRotation(
			Vector3(
				0.0f,
				0.0f,
				planets[i]->Info.ObliquityToOrbit
			)
		);

		planets[i]->ResetSatelitesPositions();
	}
}

std::vector<PlanetInfo> PlanetSystemComponent::CreatePlanetsFromFile(std::string fileName)
{
	std::ifstream fin(fileName);

	if (fin.fail()) 
	{
		throw "FailToOpenFile";
	}

	std::vector<PlanetInfo> infos;

	int count;
	std::string title;

	fin >> count;

	for (int i = 0; i < 10; i++)
		fin >> title;

	std::string name;
	float mass, diameter, density, gravity, rotPer, lengthOfday, orbRad, orbPer, obliquityToOrbit;

	//Read and add planets
	for (int i = 0; i < count; i++) 
	{
		PlanetInfo info = {};
		fin >> name >> mass >> diameter >> density >> gravity >> rotPer >> lengthOfday >> orbRad >> orbPer >> obliquityToOrbit;
		info.Name = name;
		info.Mass = mass;
		info.Diameter = diameter;
		info.Density = density;
		info.Gravity = gravity;
		info.RotationPeriod = rotPer;
		info.LengthOfDay = lengthOfday;
		info.OrbitRadius = orbRad;
		info.OrbitPeriod = orbPer;
		info.ObliquityToOrbit = obliquityToOrbit;

		infos.push_back(info);
	}

	return infos;
}
