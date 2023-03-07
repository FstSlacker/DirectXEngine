#include "Game.h"
#include "KatamariComponent.h"

int main()
{
	Game game;
	game.Name = L"Game framework";

	PixelShader* ps = new PixelShader(L"./Shaders/DefaultTexture.hlsl");
	VertexShader* vs = new VertexShader(L"./Shaders/DefaultTexture.hlsl");
	//Texture* texWall = new Texture("./Textures/brich_wall.jpg");
	Texture* texGrass = new Texture("./Textures/grass-texture.jpg");
	Texture* texMoon = new Texture("./Textures/moon.jpg");

	/*SphereComponent* s1 = new SphereComponent(&game);
	s1->Name = "Sphere1";
	s1->Collider = new SphereCollider(s1);

	s1->SetShaders(vs, ps);
	s1->SetTexture(texWall);

	SphereComponent* s2 = new SphereComponent(&game);
	s2->Name = "Sphere2";
	s2->Collider = new AABBCollider(s2);

	s2->SetShaders(vs, ps);
	s2->SetTexture(texWall);

	s2->Transform.SetPosition(Vector3(2.0f, 0.0f, 0.0f));

	s1->Transform.AddChild(s2->Transform);
*/
	game.Gfx.AddPixelShader(ps);
	game.Gfx.AddVertexShader(vs);
	game.Gfx.AddTexture(texGrass);
	game.Gfx.AddTexture(texMoon);

	ModelComponent* model = new ModelComponent(
		&game,
		"D:\\Documents\\3DModels\\Mashroom1.fbx"
	);
	model->Transform.SetScale(Vector3(0.01f, 0.01f, 0.01f));
	model->Collider = new SphereCollider(model, 50.0f);
	model->Transform.SetPosition(Vector3(10.0f, 0.0f, 10.0f));

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->SetShaders(vs, ps);
	plane->SetTexture(texGrass);
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	SphereComponent* sp1 = new SphereComponent(&game);
	sp1->Transform.SetPosition(Vector3(5.0f, 0.5f, 0.0f));
	sp1->Collider = new SphereCollider(sp1);
	sp1->SetShaders(vs, ps);
	sp1->SetTexture(texMoon);

	//CameraMoveComponent* moveComp = new CameraMoveComponent(&game);
	//moveComp->Name = "CameraMoveComp";

	KatamariComponent* katamari = new KatamariComponent(&game);

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}