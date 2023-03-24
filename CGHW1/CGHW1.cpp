#include "Game.h"
#include "ResourceManager.h"

int main()
{
	Game game;
	game.Name = L"Game framework";

	//game.Gfx.AddPixelShader(L"PS_DefaultLit.hlsl");
	//game.Gfx.AddPixelShader(L"PS_DefaultUnlit.hlsl");
	//game.Gfx.AddVertexShader(L"VS_Default.hlsl");

	PixelShader* psLit = new PixelShader(L"./Shaders/PS_DefaultLit.hlsl");
	VertexShader* vsLit = new VertexShader(L"./Shaders/VS_Default.hlsl");

	game.Resources.AddResource<VertexShader>(vsLit, "VS_Default");
	game.Resources.AddResource<PixelShader>(psLit, "PS_DefaultLit");

	Texture* texGrass = new Texture(&game, "./Textures/grass-texture.jpg");

	Texture* texWall = new Texture(&game, "./Textures/diffusemap_brick.png");
	//Texture* texNormalMapWall = new Texture(&game, "./Textures/normalmap_brick.png");
	Texture* texSpecularMapWall = new Texture(&game, "./Textures/specularmap_brick.png");

	Material* matLit1 = new Material(&game, vsLit, psLit);
	matLit1->DiffuseTexture = texGrass;

	Material* matWall = new Material(&game, vsLit, psLit);
	matWall->DiffuseTexture = texWall;
	//matWall->NormalMapTexture = texNormalMapWall;
	matWall->SpecularMapTexture = texSpecularMapWall;

	Material* defaultMat = new Material(&game, vsLit, psLit);

	//ModelComponent* model = new ModelComponent(
	//	&game,
	//	"D:\\Documents\\3DModels\\stones-blocks-asset\\stones-blocks.fbx"
	//);

	//model->Transform.SetPosition(Vector3(10.0f, 0.0f, 0.0f));
	//model->Transform.SetScale(Vector3(0.02f, 0.02f, 0.02f));
	CubeComponent* cube1 = new CubeComponent(&game);
	cube1->SetMaterial(matWall);
	cube1->Transform.SetPosition(Vector3(2.0f, 2.0f, 0.0f));

	CubeComponent* cube2 = new CubeComponent(&game);
	cube2->SetMaterial(matWall);
	cube2->Transform.SetPosition(Vector3(-2.0f, 2.0f, 0.0f));

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->SetMaterial(matLit1);
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	PointLightComponent* light = new PointLightComponent(&game);
	light->Transform.SetPosition(Vector3(0.0f, 5.0f, 0.0f));
	light->Range = 30.0f;

	SpotLightComponent* light2 = new SpotLightComponent(&game);
	light2->Transform.SetPosition(Vector3(0.0f, 5.0f, 5.0f));
	light2->SetEnable(false);

	DirectionalLightComponent* light3 = new DirectionalLightComponent(&game);
	light3->Transform.SetPosition(Vector3(0.0f, 10.0f, 10.0f));
	light3->SetEnable(false);

	CameraMoveComponent* moveComp = new CameraMoveComponent(&game);

	moveComp->Name = "CameraMoveComp";

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}