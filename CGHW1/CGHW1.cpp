#include "Game.h"

int main()
{
	Game game = Game();
	game.Name = L"Solar System";

	std::vector<PlanetInfo> infos = PlanetSystemComponent::CreatePlanetsFromFile("planets_data_transp.txt");

	PlanetComponent* sun = new PlanetComponent(
		&game,
		Vector3(0.0f, 0.0f, 0.0f),
		PlanetInfo{
			"Sun",
			0.0f,
			10.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f
		},
		Texture(L"./Textures/sun.jpg")
	);
	sun->IsStar = true;

	PlanetComponent* mercury = new PlanetComponent(
		&game,
		Vector3(10.0f, 0.0f, 0.0f),
		infos[0],
		Texture(L"./Textures/mercury.jpg")
	);

	PlanetComponent* venus = new PlanetComponent(
		&game,
		Vector3(20.0f, 0.0f, 0.0f),
		infos[1],
		Texture(L"./Textures/venus.jpg")
	);
	venus->Name = "Venus";

	PlanetComponent* earth = new PlanetComponent(
		&game,
		Vector3(30.0f, 0.0f, 0.0f),
		infos[2],
		Texture(L"./Textures/earth.jpg")
	);

	PlanetComponent* mars = new PlanetComponent(
		&game,
		Vector3(40.0f, 0.0f, 0.0f),
		infos[3],
		Texture(L"./Textures/mars.jpg")
	);

	PlanetComponent* jupiter = new PlanetComponent(
		&game,
		Vector3(50.0f, 0.0f, 0.0f),
		infos[4],
		Texture(L"./Textures/jupiter.jpg")
	);

	PlanetComponent* saturn = new PlanetComponent(
		&game,
		Vector3(60.0f, 0.0f, 0.0f),
		infos[5],
		Texture(L"./Textures/saturn.jpg")
	);

	PlanetComponent* uranus = new PlanetComponent(
		&game,
		Vector3(70.0f, 0.0f, 0.0f),
		infos[6],
		Texture(L"./Textures/uranus.jpg")
	);

	PlanetComponent* neptune = new PlanetComponent(
		&game,
		Vector3(80.0f, 0.0f, 0.0f),
		infos[7],
		Texture(L"./Textures/neptune.jpg")
	);

	Text2DComponent* text = new Text2DComponent(
		&game,
		Transform3D(
			Vector3(0.0f, 218.0f, 0.0f),
			Vector3::Zero,
			Vector3::One
		)
	);

	PlanetSystemComponent* sysComp = new PlanetSystemComponent(
		&game,
		Transform3D(),
		sun,
		std::vector<PlanetComponent*>{
			mercury,
			venus,
			earth,
			mars,
			jupiter,
			saturn,
			uranus,
			neptune
		}
	);

	CameraMoveComponent* moveComp = new CameraMoveComponent(
		&game,
		Transform3D()
	);

	

	game.Components.push_back(moveComp);

	game.Components.push_back(sun);
	game.Components.push_back(mercury);
	game.Components.push_back(venus);
	game.Components.push_back(earth);
	//game.Components.push_back(moon);
	game.Components.push_back(mars);
	game.Components.push_back(jupiter);
	game.Components.push_back(saturn);
	game.Components.push_back(uranus);
	game.Components.push_back(neptune);

	game.Components.push_back(sysComp);

	game.Components.push_back(text);

	game.ImGUI.AddWindow(new ImGuiPlanetSystem(&game, sysComp));
	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}