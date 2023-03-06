#include "PlanetComponent.h"
#include "Game.h"

void PlanetComponent::RotateSatelite(PlanetComponent* p, float speed)
{
	float orbSpeed = 360.0f / (p->Info.OrbitPeriod * 24.0f);
	float rotSpeed = 360.0f / p->Info.RotationPeriod;

	Vector3 angle = Vector3(0.0f, orbSpeed * speed, orbSpeed * speed);

	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	Vector3 posParent = Transform.GetPosition();
	p->Transform.AddPosition(posParent * -1.0f);

	DirectX::XMVECTOR scale, rot, pos;
	DirectX::XMMatrixDecompose(&scale, &rot, &pos, p->Transform.GetTransformMatrix() * rotMat);


	Vector3 newPos = Vector3(
		DirectX::XMVectorGetX(pos),
		DirectX::XMVectorGetY(pos),
		DirectX::XMVectorGetZ(pos)
	);

	p->Transform.SetPosition(newPos);
	p->Transform.AddPosition(posParent);
	p->Transform.AddLocalRotation(Vector3(0.0f, rotSpeed * speed, 0.0f));
}

PlanetComponent::PlanetComponent(Game* game, Vector3 position, PlanetInfo info, Texture* tex)
	: SphereComponent(game, Transform3D(position, Vector3::Zero, Vector3(info.Diameter, info.Diameter, info.Diameter)))
{
	SetTexture(tex);
	this->Info = info;
	this->Name = info.Name;
}

void PlanetComponent::RotateSatelites(float speed)
{
	for (int i = 0; i < satelites.size(); i++)
	{
		RotateSatelite(satelites[i], speed);
	}
}

void PlanetComponent::MoveSatelites(Vector3 addPos)
{
	for (int i = 0; i < satelites.size(); i++)
	{
		satelites[i]->Transform.AddPosition(addPos);
	}
}

void PlanetComponent::AddSatelite(PlanetComponent* sat)
{
	satelites.push_back(sat);
	sat->Transform.SetPosition(this->Transform.GetPosition() + Vector3(2.0f, 0.0f, 0.0f));
}

void PlanetComponent::ResetSatelitesPositions()
{
	for (int i = 0; i < satelites.size(); i++)
	{
		satelites[i]->Transform.SetScale(
			Vector3(
				satelites[i]->Info.Diameter,
				satelites[i]->Info.Diameter,
				satelites[i]->Info.Diameter
			)
		);
		satelites[i]->Transform.SetPosition(
			this->Transform.GetPosition() + Vector3(satelites[i]->Info.OrbitRadius, 0.0f, 0.0f));
	}
}

void PlanetComponent::Update()
{
	SphereComponent::Update();
}

void PlanetComponent::Initialize()
{
	SphereComponent::Initialize();
	ResetSatelitesPositions();
}
