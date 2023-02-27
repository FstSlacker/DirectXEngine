#pragma once
#include "ImGuiWindow.h"
#include "PlanetSystemComponent.h"

class ImGuiPlanetSystem : public ImGuiBaseWindow
{
private:
	PlanetSystemComponent* planetSystem;
	Game* game;
	bool isLighting;
	float orbScale;

public:
	ImGuiPlanetSystem(Game* game, PlanetSystemComponent* planetSys);
	virtual void Bind() override;
};

class ImGuiPlanetInfoWindow : public ImGuiBaseWindow
{
private:
	PlanetComponent* planet;
	PlanetSystemComponent* planetSystem;

public:
	ImGuiPlanetInfoWindow(PlanetComponent* planet, PlanetSystemComponent* planetSys);
	virtual void Bind() override;
};

