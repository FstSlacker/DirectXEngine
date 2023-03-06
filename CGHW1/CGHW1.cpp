#include "Game.h"

int main()
{
	Game game = Game();
	game.Name = L"Game framework";

	PixelShader* ps = new PixelShader(L"./Shaders/DefaultTexture.hlsl");
	VertexShader* vs = new VertexShader(L"./Shaders/DefaultTexture.hlsl");
	Texture* texWall = new Texture("./Textures/brich_wall.jpg");
	Texture* texGrass = new Texture("./Textures/grass-texture.jpg");

	SphereComponent* s1 = new SphereComponent(&game);
	s1->Name = "Sphere1";

	s1->SetShaders(vs, ps);
	s1->SetTexture(texWall);

	SphereComponent* s2 = new SphereComponent(&game);
	s2->Name = "Sphere2";

	s2->SetShaders(vs, ps);
	s2->SetTexture(texWall);

	s2->Transform.SetPosition(Vector3(2.0f, 0.0f, 0.0f));

	s1->Transform.AddChild(s2->Transform);

	game.Gfx.AddPixelShader(ps);
	game.Gfx.AddVertexShader(vs);
	game.Gfx.AddTexture(texWall);
	game.Gfx.AddTexture(texGrass);

	ModelComponent* model = new ModelComponent(
		&game,
		"D:\\Documents\\3DModels\\Mashroom1.fbx"
	);
	model->Transform.SetRotation(Vector3(90.0f, 180.0f, 0.0f));

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->SetShaders(vs, ps);
	plane->SetTexture(texGrass);
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	CameraMoveComponent* moveComp = new CameraMoveComponent(&game);

	moveComp->Name = "CameraMoveComp";

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}