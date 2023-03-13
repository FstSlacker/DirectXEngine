#include "Game.h"
#include "KatamariComponent.h"

int main()
{
	Game game;
	game.Name = L"Katamari";

	game.Gfx.AddPixelShader(L"PS_DefaultLit.hlsl");
	game.Gfx.AddVertexShader(L"VS_Default.hlsl");

	Texture* texGrass = new Texture(&game, "./Textures/grass-texture.jpg");
	Texture* texMoon = new Texture(&game, "./Textures/moon.jpg");
	Texture* texNorm = new Texture(&game, "./Textures/normal_map3.jpg");

	Material* groundMat = new Material(&game, game.Gfx.FindVertexShader(L"VS_Default.hlsl"), game.Gfx.FindPixelShader(L"PS_DefaultLit.hlsl"));
	groundMat->DiffuseTexture = texGrass;
	groundMat->NormalMapTexture = texNorm;
	groundMat->SpecularColor = Color(0.2f, 0.2f, 0.2f);
	
	Material* moonMat = new Material(&game, game.Gfx.FindVertexShader(L"VS_Default.hlsl"), game.Gfx.FindPixelShader(L"PS_DefaultLit.hlsl"));
	moonMat->DiffuseTexture = texMoon;
	moonMat->SpecularColor = Color(0.0f);

	std::string modelsPaths[] = {
		"D:\\Documents\\3DModels\\Mashroom1.fbx",
		"D:\\Documents\\3DModels\\Mashroom2.fbx",
		"D:\\Documents\\3DModels\\Mashroom3.fbx",
		"D:\\Documents\\3DModels\\game-ready-low-poly-old-tv-cartoon-style-model\\source\\TV.fbx"
	};

	for (int j = 0; j < 1; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			ModelComponent* model = new ModelComponent(
				&game,
				modelsPaths[i]
			);
			Vector3 pos = Vector3(
				(float)(rand() % 1000) * 0.01f * (rand() % 2 == 0 ? -1.0f : 1.0f),
				0.0f,
				(float)(rand() % 1000) * 0.01f * (rand() % 2 == 0 ? -1.0f : 1.0f)
			);
			if (i < 3)
			{
				model->Transform.SetScale(Vector3(0.01f, 0.01f, 0.01f));
				model->Collider = new SphereCollider(model, 50.0f);
			}
			else
			{
				model->Collider = new SphereCollider(model, 1.0f, Vector3(0.0f, 0.0f, -1.0f));
			}
			model->Transform.SetPosition(pos);
			model->Transform.SetRotation(Vector3(0.0f, (float)(rand() % 360), 0.0f));
		}
	}

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->Material = groundMat;
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	SphereComponent* sp1 = new SphereComponent(&game);
	sp1->Transform.SetPosition(Vector3(5.0f, 0.5f, 0.0f));
	sp1->Collider = new SphereCollider(sp1);
	sp1->Material = moonMat;

	DirectionalLightComponent* dirLight = new DirectionalLightComponent(&game);
	dirLight->Transform.SetRotation(Vector3(50.0f, 0.0f, 0.0f));

	KatamariComponent* katamari = new KatamariComponent(&game);

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}