#include "Game.h"

int main()
{
	Game game;
	game.Name = L"Game framework";

	game.Gfx.AddPixelShader(L"PS_DefaultLit.hlsl");
	game.Gfx.AddPixelShader(L"PS_DefaultUnlit.hlsl");
	game.Gfx.AddVertexShader(L"VS_Default.hlsl");

	Texture* texWall = new Texture(&game, "./Textures/brich_wall.jpg");
	Texture* texGrass = new Texture(&game, "./Textures/obsidiant.jpg");

	Material* matLit1 = new Material(&game, game.Gfx.FindVertexShader(L"VS_Default.hlsl"), game.Gfx.FindPixelShader(L"PS_DefaultLit.hlsl"));
	matLit1->DiffuseTexture = texGrass;
	Material* matLit2 = new Material(&game, game.Gfx.FindVertexShader(L"VS_Default.hlsl"), game.Gfx.FindPixelShader(L"PS_DefaultLit.hlsl"));
	matLit2->DiffuseTexture = texWall;
	Material* matUnlit1 = new Material(&game, game.Gfx.FindVertexShader(L"VS_Default.hlsl"), game.Gfx.FindPixelShader(L"PS_DefaultUnlit.hlsl"));
	matUnlit1->DiffuseTexture = texWall;


	SphereComponent* s1 = new SphereComponent(&game);
	s1->Name = "Sphere1";
	s1->Collider = new SphereCollider(s1);

	s1->Material = matLit1;

	SphereComponent* s2 = new SphereComponent(&game);
	s2->Name = "Sphere2";
	s2->Collider = new AABBCollider(s2);

	s2->Material = matUnlit1;

	s2->Transform.SetPosition(Vector3(2.0f, 0.0f, 0.0f));

	s1->Transform.AddChild(s2->Transform);

	ModelComponent* model = new ModelComponent(
		&game,
		"D:\\Documents\\3DModels\\Mashroom1.fbx"
	);
	model->Transform.SetPosition(Vector3(3.0f, 0.0f, 0.0f));
	model->Transform.SetScale(Vector3(0.02f, 0.02f, 0.02f));

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->Material = matLit2;
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	PointLightComponent* light = new PointLightComponent(&game);
	light->Transform.SetPosition(Vector3(10.0f, 10.0f, 0.0f));
	light->Range = 30.0f;

	SpotLightComponent* light2 = new SpotLightComponent(&game);
	light2->Transform.SetPosition(Vector3(-10.0f, 10.0f, 0.0f));

	DirectionalLightComponent* light3 = new DirectionalLightComponent(&game);
	light3->Transform.SetPosition(Vector3(0.0f, 10.0f, 10.0f));

	CameraMoveComponent* moveComp = new CameraMoveComponent(&game);

	moveComp->Name = "CameraMoveComp";

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();

	//system("pause");
}