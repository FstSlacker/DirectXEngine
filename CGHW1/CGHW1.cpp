#include "Game.h"
#include "ResourceManager.h"

int main()
{
	Game game;
	game.Name = L"Game framework";

	game.Gizmos.ShowObjectsGizmos = true;

	PixelShader* psLit = new PixelShader(L"./Shaders/PS_DefaultShadowLit.hlsl");
	VertexShader* vsLit = new VertexShader(L"./Shaders/VS_DefaultShadow.hlsl");

	//PixelShader* psDepth = new PixelShader(L"./Shaders/PS_Depth.hlsl");
	//VertexShader* vsDepth = new VertexShader(L"./Shaders/VS_Depth.hlsl");

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

	//Material* defaultMat = new Material(&game, vsLit, psLit);

	//ModelComponent* model = new ModelComponent(
	//	&game,
	//	"D:\\Documents\\3DModels\\Battle_Tower_FBX\\FBX\\War_Tower.fbx"
	//);

	//model->Transform.SetPosition(Vector3(10.0f, 0.0f, 0.0f));
	//model->Transform.SetRotation(Vector3(90.0f, 180.0f, 0.0f));
	//model->Transform.SetScale(Vector3(1.0f, 1.0f, 1.0f));

	CubeComponent* cube1 = new CubeComponent(&game);
	cube1->SetMaterial(matWall);
	cube1->Transform.SetPosition(Vector3(12.5f, 0.0f, -25.0f));
	cube1->Transform.SetScale(Vector3(25.0f, 10.0f, 2.0f));

	CubeComponent* cube12 = new CubeComponent(&game);
	cube12->SetMaterial(matWall);
	cube12->Transform.SetPosition(Vector3(-12.5f, 0.0f, -25.0f));
	cube12->Transform.SetScale(Vector3(25.0f, 10.0f, 2.0f));

	CubeComponent* cube2 = new CubeComponent(&game);
	cube2->SetMaterial(matWall);
	cube2->Transform.SetPosition(Vector3(25.0f, 0.0f, -12.5f));
	cube2->Transform.SetScale(Vector3(2.0f, 10.0f, 25.0f));

	CubeComponent* cube22 = new CubeComponent(&game);
	cube22->SetMaterial(matWall);
	cube22->Transform.SetPosition(Vector3(25.0f, 0.0f, 12.5f));
	cube22->Transform.SetScale(Vector3(2.0f, 10.0f, 25.0f));

	CubeComponent* cube3 = new CubeComponent(&game);
	cube3->SetMaterial(matWall);
	cube3->Transform.SetPosition(Vector3(-25.0f, 0.0f, -12.5f));
	cube3->Transform.SetScale(Vector3(2.0f, 10.0f, 25.0f));

	CubeComponent* cube32 = new CubeComponent(&game);
	cube32->SetMaterial(matWall);
	cube32->Transform.SetPosition(Vector3(-25.0f, 0.0f, 12.5f));
	cube32->Transform.SetScale(Vector3(2.0f, 10.0f, 25.0f));

	CubeComponent* cube4 = new CubeComponent(&game);
	cube4->SetMaterial(matWall);
	cube4->Transform.SetPosition(Vector3(-12.5f, 0.0f, 25.0f));
	cube4->Transform.SetScale(Vector3(25.0f, 10.0f, 2.0f));

	CubeComponent* cube42 = new CubeComponent(&game);
	cube42->SetMaterial(matWall);
	cube42->Transform.SetPosition(Vector3(12.5f, 0.0f, 25.0f));
	cube42->Transform.SetScale(Vector3(25.0f, 10.0f, 2.0f));

	//cube1->GetMesh()->RecalculateTangents();

	//CubeComponent* cube2 = new CubeComponent(&game);
	//cube2->SetMaterial(matLit1);
	//cube2->Transform.SetPosition(Vector3(5.0f, 2.0f, 5.0f));
	//cube2->Transform.SetScale(Vector3(3.0f, 3.0f, 3.0f));

	//SphereComponent* sphere1 = new SphereComponent(&game);
	//sphere1->SetMaterial(matWall);
	//sphere1->Transform.SetPosition(Vector3(5.0f, 2.0f, -5.0f));
	//sphere1->Transform.SetScale(Vector3(2.0f, 2.0f, 2.0f));

	//SphereComponent* sphere2 = new SphereComponent(&game);
	//sphere2->SetMaterial(matLit1);
	//sphere2->Transform.SetPosition(Vector3(-5.0f, 2.0f, -5.0f));
	//sphere2->Transform.SetScale(Vector3(2.0f, 2.0f, 2.0f));

	//CubeComponent* cubeWall = new CubeComponent(&game);
	//cubeWall->SetMaterial(matWall);
	//cubeWall->Transform.SetPosition(Vector3(0.0f, 15.0f, 10.0f));
	//cubeWall->Transform.SetScale(Vector3(30.0f, 30.0f, 3.0f));

	PlaneComponent* plane = new PlaneComponent(&game);
	plane->SetMaterial(matLit1);
	plane->Transform.SetScale(Vector3(50.0f, 50.0f, 50.0f));

	plane->Transform.AddChild(cube1->Transform);
	plane->Transform.AddChild(cube12->Transform);
	plane->Transform.AddChild(cube2->Transform);
	plane->Transform.AddChild(cube22->Transform);
	plane->Transform.AddChild(cube3->Transform);
	plane->Transform.AddChild(cube32->Transform);
	plane->Transform.AddChild(cube4->Transform);
	plane->Transform.AddChild(cube42->Transform);

	AmbientLightComponent* ambientLight = new AmbientLightComponent(&game);
	DirectionalLightComponent* directionalLight = new DirectionalLightComponent(&game);
	directionalLight->Transform.SetRotation(Vector3(30.0f, 0.0f, 0.0f));

	PointLightComponent* light = new PointLightComponent(&game);
	light->Transform.SetPosition(Vector3(0.0f, 20.0f, 0.0f));
	light->Range = 30.0f;
	light->LightColor = Color(1.0f, 1.0f, 1.0f);

	/*PointLightComponent* light3 = new PointLightComponent(&game);
	light3->Transform.SetPosition(Vector3(5.0f, 5.0f, 0.0f));
	light3->Range = 10.0f;
	light3->LightColor = Color(1.0f, 0.0f, 0.0f);

	SpotLightComponent* light2 = new SpotLightComponent(&game);
	light2->Transform.SetPosition(Vector3(5.0f, 10.0f, 0.0f));
	light2->LightColor = Color(0.0f, 0.0f, 1.0f);
	light2->Range = 20.0f;

	SpotLightComponent* light4 = new SpotLightComponent(&game);
	light4->Transform.SetPosition(Vector3(-5.0f, 10.0f, 0.0f));
	light4->LightColor = Color(0.0f, 1.0f, 1.0f);
	light4->Range = 20.0f;

	MeshComponent* cone = new MeshComponent(&game);
	cone->SetMesh(Mesh::CreateSimpleConeMesh(16U, 5.0f, 10.0f));
	cone->Transform.SetPosition(Vector3(0.0f, 10.0f, -5.0f));
	cone->Transform.SetRotation(Vector3(90.0f, 0.0f, 0.0f));
	cone->SetMaterial(matLit1);*/

	ParticleSystem* ps = new ParticleSystem(&game);

	CameraMoveComponent* moveComp = new CameraMoveComponent(&game);

	moveComp->Name = "CameraMoveComp";

	game.ImGUI.AddWindow(new ImGuiGameInfoWindow(&game));

	game.Run();
}