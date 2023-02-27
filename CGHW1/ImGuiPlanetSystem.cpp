#include "ImGuiPlanetSystem.h"
#include "Game.h"

ImGuiPlanetSystem::ImGuiPlanetSystem(Game* game, PlanetSystemComponent* planetSys) : ImGuiBaseWindow("Solar system")
{
	this->planetSystem = planetSys;
	this->game = game;
	this->isLighting = true;
	this->orbScale = 1.0f;
}

void ImGuiPlanetSystem::Bind()
{
	ImGui::Text("Simulation speed (1 real second =):");
	ImGui::SliderFloat("x", &planetSystem->SimulationSpeed, 1.0f, 36000.0f);
	if (ImGui::SliderFloat("Orbit scale", &orbScale, 0.01f, 5.0f))
	{
		planetSystem->SetOrbitScale(orbScale);
	}
	if (ImGui::Checkbox("Lighting On/Off", &isLighting))
	{
		planetSystem->SetLighting(isLighting);
	}
	if (ImGui::Button(planetSystem->star->Name.c_str()))
	{
		game->ImGUI.AddWindow(new ImGuiPlanetInfoWindow(planetSystem->star, planetSystem));
	}

	ImGui::Spacing();
	if (ImGui::TreeNode("Planets (8)"))
	{
		for (int i = 0; i < planetSystem->planets.size(); i++)
		{
			if (ImGui::Button(planetSystem->planets[i]->Name.c_str()))
			{
				game->ImGUI.AddWindow(new ImGuiPlanetInfoWindow(planetSystem->planets[i], planetSystem));
			}
		}
		ImGui::TreePop();
		ImGui::Spacing();
	}

	if (ImGui::Button("Reset system"))
	{
		planetSystem->ResetSystem();
	}
}

ImGuiPlanetInfoWindow::ImGuiPlanetInfoWindow(PlanetComponent* planet, PlanetSystemComponent* planetSys) : ImGuiBaseWindow(planet->Name.c_str())
{
	this->planet = planet;
	this->planetSystem = planetSys;
}

void ImGuiPlanetInfoWindow::Bind()
{
	ImGui::Text("Mass: %.2f (10^24 kg)", planet->Info.Mass);
	ImGui::Text("Diameter: %.2f (km)", planet->Info.Diameter);
	ImGui::Text("Density: %.2f (kg/m^3)", planet->Info.Density);
	ImGui::Text("Gravity: %.2f (m/s^2)", planet->Info.Gravity);
	ImGui::Text("Rotation periud: %.2f (hours)", planet->Info.RotationPeriod);
	ImGui::Text("Day length: %.2f (hours)", planet->Info.LengthOfDay);
	ImGui::Text("Orbit radius: %.2f (10^6 km)", planet->Info.OrbitRadius);
	ImGui::Text("Orbit periud: %.2f (days)", planet->Info.OrbitPeriod);
	ImGui::Text("Obliquity to orbit: %.2f (deg)", planet->Info.ObliquityToOrbit);

	if (ImGui::Button("Follow"))
	{
		planetSystem->StartFollowPlanet(planet);
	}
	ImGui::SameLine();
	if (ImGui::Button("Unfollow"))
	{
		planetSystem->UnfollowPlanet();
	}
}
