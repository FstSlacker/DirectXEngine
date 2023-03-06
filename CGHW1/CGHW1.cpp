#include "Game.h"

int main()
{
	Game game = Game();
	game.Name = L"Solar System";

	PixelShader* psTex = new PixelShader(L"./Shaders/DefaultTexture.hlsl");
	VertexShader* vsTex = new VertexShader(L"./Shaders/DefaultTexture.hlsl");

	game.Gfx.AddPixelShader(psTex);
	game.Gfx.AddVertexShader(vsTex);

	Texture* texSun = new Texture(L"./Textures/sun.jpg");
	Texture* texMoon = new Texture(L"./Textures/moon.jpg");
	Texture* texMercury = new Texture(L"./Textures/mercury.jpg");
	Texture* texVenus = new Texture(L"./Textures/venus.jpg");
	Texture* texEarth = new Texture(L"./Textures/earth.jpg");
	Texture* texMars = new Texture(L"./Textures/mars.jpg");
	Texture* texJupiter = new Texture(L"./Textures/jupiter.jpg");
	Texture* texSaturn = new Texture(L"./Textures/saturn.jpg");
	Texture* texUranus = new Texture(L"./Textures/uranus.jpg");
	Texture* texNeptune = new Texture(L"./Textures/neptune.jpg");

	game.Gfx.AddTexture(texSun);
	game.Gfx.AddTexture(texMoon);
	game.Gfx.AddTexture(texMercury);
	game.Gfx.AddTexture(texVenus);
	game.Gfx.AddTexture(texEarth);
	game.Gfx.AddTexture(texMars);
	game.Gfx.AddTexture(texJupiter);
	game.Gfx.AddTexture(texSaturn);
	game.Gfx.AddTexture(texUranus);
	game.Gfx.AddTexture(texNeptune);

	std::vector<PlanetInfo> infos = PlanetSystemComponent::CreatePlanetsFromFile("planets_data_transp.txt");


	PlanetInfo moonInfo = {};
	moonInfo.Diameter = 0.5f;
	moonInfo.OrbitRadius = 2.0f;
	moonInfo.OrbitPeriod = 10.0f;
	moonInfo.RotationPeriod = 1.0f;
	moonInfo.Name = "Moon";

	PlanetComponent* moon = new PlanetComponent(
		&game,
		Vector3(0.0f, 0.0f, 0.0f),
		moonInfo,
		texMoon
	);

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
		texSun
	);

	PlanetComponent* mercury = new PlanetComponent(
		&game,
		Vector3(10.0f, 0.0f, 0.0f),
		infos[0],
		texMercury
	);

	PlanetComponent* venus = new PlanetComponent(
		&game,
		Vector3(20.0f, 0.0f, 0.0f),
		infos[1],
		texVenus
	);

	PlanetComponent* earth = new PlanetComponent(
		&game,
		Vector3(30.0f, 0.0f, 0.0f),
		infos[2],
		texEarth
	);

	earth->AddSatelite(moon);

	PlanetComponent* mars = new PlanetComponent(
		&game,
		Vector3(40.0f, 0.0f, 0.0f),
		infos[3],
		texMars
	);

	PlanetComponent* jupiter = new PlanetComponent(
		&game,
		Vector3(50.0f, 0.0f, 0.0f),
		infos[4],
		texJupiter
	);

	PlanetComponent* saturn = new PlanetComponent(
		&game,
		Vector3(60.0f, 0.0f, 0.0f),
		infos[5],
		texSaturn
	);

	PlanetComponent* uranus = new PlanetComponent(
		&game,
		Vector3(70.0f, 0.0f, 0.0f),
		infos[6],
		texUranus
	);

	PlanetComponent* neptune = new PlanetComponent(
		&game,
		Vector3(80.0f, 0.0f, 0.0f),
		infos[7],
		texNeptune
	);

	moon->SetShaders(vsTex, psTex);
	sun->SetShaders(vsTex, psTex);
	mercury->SetShaders(vsTex, psTex);
	venus->SetShaders(vsTex, psTex);
	earth->SetShaders(vsTex, psTex);
	mars->SetShaders(vsTex, psTex);
	jupiter->SetShaders(vsTex, psTex);
	saturn->SetShaders(vsTex, psTex);
	uranus->SetShaders(vsTex, psTex);
	neptune->SetShaders(vsTex, psTex);

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

	game.Components.push_back(moon);
	game.Components.push_back(sun);
	game.Components.push_back(mercury);
	game.Components.push_back(venus);
	game.Components.push_back(earth);
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