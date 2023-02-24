#include "CameraMoveComponent.h"
#include "Game.h"

CameraMoveComponent::CameraMoveComponent(Game* game, Transform3D transform) : GameComponent(game, transform)
{

}

void CameraMoveComponent::Initialize()
{
	prevMousePos = game->Input->MousePosition;
}

void CameraMoveComponent::Update()
{
	Transform3D* camT = &game->MainCamera->Transform;

	Vector2 currPos = game->Input->MousePosition;
	Vector2 deltaPos = currPos - prevMousePos;
	prevMousePos = currPos;


	if (game->Input->IsKeyDown(Keys::W))
	{
		camT->AddPosition(camT->GetForward() * game->DeltaTime);
	}
	if (game->Input->IsKeyDown(Keys::A))
	{
		camT->AddPosition(camT->GetLeft() * game->DeltaTime);
	}
	if (game->Input->IsKeyDown(Keys::S))
	{
		camT->AddPosition(camT->GetBackward() * game->DeltaTime);
	}
	if (game->Input->IsKeyDown(Keys::D))
	{
		camT->AddPosition(camT->GetRight() * game->DeltaTime);
	}

	float density = 0.2f;
	camT->AddRotation(Vector3(deltaPos.y * density, deltaPos.x * density, 0.0f));
}
