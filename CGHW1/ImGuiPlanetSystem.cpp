#include "ImGuiPlanetSystem.h"
#include "Game.h"

ImGuiPlanetSystem::ImGuiPlanetSystem(Game* game, PlanetSystemComponent* planetSys) : ImGuiBaseWindow("Solar system")
{
	this->planetSystem = planetSys;
	this->game = game;
}

void ImGuiPlanetSystem::Bind()
{

	ImGui::SliderFloat("Simulation speed", &planetSystem->SimulationSpeed, 0.01f, 5.0f);
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
}

ImGuiPlanetInfoWindow::ImGuiPlanetInfoWindow(PlanetComponent* planet, PlanetSystemComponent* planetSys) : ImGuiBaseWindow(planet->Name.c_str())
{
	this->planet = planet;
	this->planetSystem = planetSys;
}

void ImGuiPlanetInfoWindow::Bind()
{
	ImGui::Text("Diameter: %.2f", planet->Transform.GetScale().x);
	ImGui::Text("Orbit radius: %.2f", planet->OrbitRadius);
	ImGui::Text("Orbit speed: %.2f", planet->OrbitSpeed);
	if (ImGui::Button("Follow"))
	{
		planetSystem->StartFollowPlanet(planet);
	}
}
