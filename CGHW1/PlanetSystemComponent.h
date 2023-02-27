#pragma once
#include "GameComponent.h"
#include "PlanetComponent.h"
#include <vector>

class ImGuiPlanetSystem;

class PlanetSystemComponent : public GameComponent
{
	friend class ImGuiPlanetSystem;

private:
	const float kSecPerHour = 0.00027778f;
	PlanetComponent* star;
	std::vector<PlanetComponent*> planets;
	bool isFollowMode;
	bool isLighting;
	PlanetComponent* followTarget;
	float orbitScale;

public:
	float SimulationSpeed;

	PlanetSystemComponent(Game* game, Transform3D transform, PlanetComponent* star, std::vector<PlanetComponent*> planets);
	void Update() override;
	void Initialize() override;
	void RotatePlanet(PlanetComponent* p);
	void StartFollowPlanet(PlanetComponent* p);
	void UnfollowPlanet();
	void ScaleDiametersRelativeEarth();
	void SetLighting(bool isActive);
	void SetOrbitScale(float orbScale);
	void ResetSystem();

	static std::vector<PlanetInfo> CreatePlanetsFromFile(std::string fileName);

};

