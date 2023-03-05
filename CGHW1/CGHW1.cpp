#include "Game.h"

int main()
{
	Game game = Game();
	game.Name = L"Solar System";

	PixelShader* ps = new PixelShader(L"./Shaders/DefaultTexture.hlsl");
	VertexShader* vs = new VertexShader(L"./Shaders/DefaultTexture.hlsl");
	Texture* tex = new Texture(L"./Textures/brich_wall.jpg");

	game.Gfx.AddPixelShader(ps);
	game.Gfx.AddVertexShader(vs);
	
	game.Gfx.AddTexture(tex);

	SphereComponent* s1 = new SphereComponent(&game);
	SphereComponent* s2 = new SphereComponent(&game);
	s1->Name = "Sphere1";
	s2->Name = "Sphere2";
	s1->SetShaders(vs, ps);
	s2->SetShaders(vs, ps);
	s1->SetTexture(tex);
	s2->SetTexture(tex);

	s1->Transform.SetPosition(Vector3(2.0f, 0.0f, 0.0f));
	s1->Transform.SetParent(&s2->Transform);

	CameraMoveComponent* moveComp = new CameraMoveComponent(
		&game
	);

	moveComp->Name = "CameraMoveComp";

	

	game.Components.push_back(moveComp);
	game.Components.push_back(s2);
	game.Components.push_back(s1);

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}