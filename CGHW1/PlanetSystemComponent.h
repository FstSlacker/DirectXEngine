#pragma once
#include "GameComponent.h"
#include "PlanetComponent.h"
#include <vector>

class PlanetSystemComponent : public GameComponent
{
private:
	PlanetComponent* star;
	std::vector<PlanetComponent*> planets;

public:
	PlanetSystemComponent(Game* game, Transform3D transform, PlanetComponent* star, std::vector<PlanetComponent*> planets);
	void Update() override;
	void Initialize() override;
	void RotatePlanet(PlanetComponent* p);

};

