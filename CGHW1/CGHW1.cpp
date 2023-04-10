#include "Game.h"
#include "ResourceManager.h"

int main()
{
	Game game;
	game.Name = L"Game framework";

	game.Gizmos.ShowObjectsGizmos = true;

	PixelShader* psLit = new PixelShader(L"./Shaders/PS_DefaultShadowLit.hlsl");
	VertexShader* vsLit = new VertexShader(L"./Shaders/VS_DefaultShadow.hlsl");

	PixelShader* psDepth = new PixelShader(L"./Shaders/PS_Depth.hlsl");
	VertexShader* vsDepth = new VertexShader(L"./Shaders/VS_Depth.hlsl");

	//game.Resources.AddResource<VertexShader>(vsLit, "VS_Default");
	//game.Resources.AddResource<PixelShader>(psLit, "PS_DefaultLit");
	//game.Resources.AddResource<VertexShader>(vsDepth, "VS_Depth");
	//game.Resources.AddResource<PixelShader>(psDepth, "PS_Depth");

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
	cube1->SetMaterial(matLit1);
	cube1->Transform.SetPosition(Vector3(-5.0f, 2.0f, 0.0f));
	cube1->Transform.SetScale(Vector3(3.0f, 3.0f, 3.0f));

	SphereComponent* sphere1 = new SphereComponent(&game);
	sphere1->SetMaterial(matWall);
	sphere1->Transform.SetPosition(Vector3(5.0f, 2.0f, 0.0f));
	sphere1->Transform.SetScale(Vector3(2.0f, 2.0f, 2.0f));

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->SetMaterial(matLit1);
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	AmbientLightComponent* ambientLight = new AmbientLightComponent(&game);
	DirectionalLightComponent* directionalLight = new DirectionalLightComponent(&game);

	//PointLightComponent* light = new PointLightComponent(&game);
	//light->Transform.SetPosition(Vector3(0.0f, 2.0f, 0.0f));
	//light->Range = 10.0f;

	//SpotLightComponent* light2 = new SpotLightComponent(&game);
	//light2->Transform.SetPosition(Vector3(0.0f, 2.0f, 0.0f));

	MeshComponent* cone = new MeshComponent(&game);
	cone->SetMesh(Mesh::CreateSimpleConeMesh(16U, 5.0f, 10.0f));
	cone->Transform.SetPosition(Vector3(0.0f, 10.0f, -5.0f));
	cone->Transform.SetRotation(Vector3(90.0f, 0.0f, 0.0f));
	cone->SetMaterial(matLit1);


	CameraMoveComponent* moveComp = new CameraMoveComponent(&game);

	moveComp->Name = "CameraMoveComp";

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}