#include "KatamariComponent.h"
#include "Game.h"
#include <math.h>

void KatamariComponent::UpdateRotation()
{
	Vector2 currPos = game->Input->MousePosition;
	Vector2 deltaPos = currPos - prevMousePos;
	prevMousePos = currPos;

	float speed = MoveSpeed * game->DeltaTime;
	float rotSpeed = (MoveSpeed / currentSize) * 360.0f * game->DeltaTime;

	Vector3 moveDir = Vector3::Zero;
	Vector3 rotDir = Vector3::Zero;

	if (game->Input->IsKeyDown(Keys::W))
	{
		moveDir += cameraRootComp->Transform.GetForward();
		rotDir += cameraRootComp->Transform.GetRight();
	}
	if (game->Input->IsKeyDown(Keys::A))
	{
		moveDir += cameraRootComp->Transform.GetLeft();
		rotDir += cameraRootComp->Transform.GetForward();
	}
	if (game->Input->IsKeyDown(Keys::S))
	{
		moveDir += cameraRootComp->Transform.GetBackward();
		rotDir += cameraRootComp->Transform.GetLeft();
	}
	if (game->Input->IsKeyDown(Keys::D))
	{
		moveDir += cameraRootComp->Transform.GetRight();
		rotDir += cameraRootComp->Transform.GetBackward();
	}

	if (moveDir != Vector3::Zero)
	{
		moveDir.Normalize();
		rotDir.Normalize();

		this->Transform.AddPosition(moveDir * speed);
		this->rootComp->Transform.RotateAroundAxis(rotDir, rotSpeed);
	}

	float density = 0.3f;
	cameraRootComp->Transform.AddLocalRotation(Vector3(0.0f, deltaPos.x * density, 0.0f));
	
}

void KatamariComponent::OnCollisionEnter(const CollisionArgs& args)
{
	this->rootComp->Transform.AddChild(args.CollidedComponent->Transform);
	targetSize += 0.5f;
}

KatamariComponent::KatamariComponent(Game* game, float startSize)
	: GameComponent(game)
{
	this->Name = "Katamari";
	this->currentSize = startSize;
	this->targetSize = startSize;
	this->growSpeed = 1.0f;
	this->MoveSpeed = 2.0f;

	PixelShader* ps = new PixelShader(L"./Shaders/DefaultTexture.hlsl");
	VertexShader* vs = new VertexShader(L"./Shaders/DefaultTexture.hlsl");
	Texture* tex = new Texture("./Textures/brich_wall.jpg");

	game->Gfx.AddPixelShader(ps);
	game->Gfx.AddVertexShader(vs);
	game->Gfx.AddTexture(tex);

	rootComp = new GameComponent(game);
	this->Transform.AddChild(rootComp->Transform);
	rootComp->Transform.SetLocalPosition(Vector3(0.0f, startSize * 0.5f, 0.0f));

	sphereComp = new SphereComponent(game);
	sphereComp->Name = "Sphere";
	this->rootComp->Transform.AddChild(sphereComp->Transform);
	sphereComp->Transform.SetLocalPosition(Vector3::Zero);
	sphereComp->Transform.SetLocalScale(Vector3(startSize, startSize, startSize));
	sphereComp->Collider = new SphereCollider(sphereComp);
	sphereComp->Collider->OnCollisionEnter.AddRaw(this, &KatamariComponent::OnCollisionEnter);
	sphereComp->SetShaders(vs, ps);
	sphereComp->SetTexture(tex);

	cameraRootComp = new GameComponent(game);
	cameraRootComp->Name = "CameraRoot";
	this->Transform.AddChild(cameraRootComp->Transform);
	cameraRootComp->Transform.SetLocalPosition(Vector3::Zero);
	cameraRootComp->Transform.AddChild(game->MainCamera->Transform);

	game->MainCamera->Transform.SetLocalPosition(Vector3(0.0f, currentSize * 3.0f, -(currentSize * 3.0f)));
	game->MainCamera->Transform.SetLocalRotation(Vector3(45.0f, 0.0f, 0.0f));
}

void KatamariComponent::Initialize()
{
	GameComponent::Initialize();
	prevMousePos = game->Input->MousePosition;
}

void KatamariComponent::Update()
{
	GameComponent::Update();
	if (currentSize != targetSize)
	{
		float sign = std::signbit(targetSize - currentSize) ? -1.0f : 1.0f;
		float inc = sign * growSpeed * game->DeltaTime;

		currentSize = abs(targetSize - currentSize) < abs(inc) ? targetSize : currentSize + inc;

		game->MainCamera->Transform.SetLocalPosition(Vector3(0.0f, currentSize * 3.0f, -(currentSize * 3.0f)));

		this->sphereComp->Transform.SetLocalScale(Vector3(currentSize, currentSize, currentSize));
		this->rootComp->Transform.SetLocalPosition(Vector3(0.0f, currentSize * 0.5f, 0.0f));
	}
	UpdateRotation();
}
