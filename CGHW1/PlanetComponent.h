#pragma once
#include "SphereComponent.h"
#include "Texture.h"
#include <GeometricPrimitive.h>
#include <Effects.h>
#include <memory>

using namespace DirectX;

struct PlanetInfo
{
	std::string Name;
	float Mass;
	float Diameter;
	float Density;
	float Gravity;
	float LengthOfDay;
	float OrbitRadius;
	float OrbitPeriod;
	float RotationPeriod;
	float ObliquityToOrbit;
};

class PlanetComponent : public SphereComponent
{
private:
	void RotateSatelite(PlanetComponent* p, float speed);

public:
	PlanetInfo Info;
	std::vector<PlanetComponent*> satelites;

	PlanetComponent(Game* game, Vector3 position, PlanetInfo info, Texture* tex);

	void RotateSatelites(float speed);
	void MoveSatelites(Vector3 addPos);
	void AddSatelite(PlanetComponent* sat);

	void ResetSatelitesPositions();
	void Update() override;
	void Initialize() override;
};

