#include "CameraMoveComponent.h"
#include "../Game.h"

CameraMoveComponent::CameraMoveComponent(Game* game) : GameComponent(game)
{
	this->Name = "CameraMover";
	this->MoveSpeed = 5.0f;
}

void CameraMoveComponent::Initialize()
{
	prevMousePos = game->Input->MousePosition;
}

void CameraMoveComponent::Update()
{
	Transform3D* camT = &Camera::Main->Transform;

	Vector2 currPos = game->Input->MousePosition;
	Vector2 deltaPos = currPos - prevMousePos;
	prevMousePos = currPos;


	if (game->Input->IsKeyDown(Keys::W))
	{
		camT->AddPosition(camT->GetForward() * game->DeltaTime * MoveSpeed);
	}
	if (game->Input->IsKeyDown(Keys::A))
	{
		camT->AddPosition(camT->GetLeft() * game->DeltaTime * MoveSpeed);
	}
	if (game->Input->IsKeyDown(Keys::S))
	{
		camT->AddPosition(camT->GetBackward() * game->DeltaTime * MoveSpeed);
	}
	if (game->Input->IsKeyDown(Keys::D))
	{
		camT->AddPosition(camT->GetRight() * game->DeltaTime * MoveSpeed);
	}
	if (game->Input->IsKeyDown(Keys::Space))
	{
		camT->AddPosition(camT->GetUp() * game->DeltaTime * MoveSpeed);
	}
	if (game->Input->IsKeyDown(Keys::LeftShift))
	{
		camT->AddPosition(camT->GetDown() * game->DeltaTime * MoveSpeed);
	}

	if (game->Input->IsKeyDown(Keys::RightButton))
	{
		float density = 0.3f;
		camT->AddRotation(Vector3(deltaPos.y * density, deltaPos.x * density, 0.0f));
	}

	
}
