#include "GameComponent.h"
#include <iostream>
#include "../Game.h"

GameComponent::GameComponent(Game* game) : Transform(*this)
{
	this->game = game;
	this->isEnabled = true;
	this->Name = "GameComponent_" + std::to_string(game->Components.size());
	this->game->Components.push_back(this);
}

void GameComponent::DestroyResources() 
{
	//...
}

void GameComponent::Draw() 
{
	//...
}

void GameComponent::Bind()
{
	//...
}

void GameComponent::Initialize() 
{
	//...
}

void GameComponent::Reaload() 
{
	//...
}

void GameComponent::Update() 
{
	//...
}

void GameComponent::FixedUpdate()
{
	//...
}

bool GameComponent::IsEnabled() const
{
	return this->isEnabled;
}

void GameComponent::SetEnable(bool isEnable)
{
	if (isEnable != this->isEnabled)
	{
		this->isEnabled = isEnable;
		for (int i = 0; i < Transform.GetChildsCount(); i++)
		{
			Transform.GetChild(i)->GetGameComponent()->SetEnable(this->isEnabled);
		}
	}
}

void GameComponent::DrawGizmos()
{
	game->Gizmos.DrawAxis(this);
	game->Gizmos.DrawCollider(this);
}

void GameComponent::DrawGizmosIcon(Vector3 right, Vector3 up, float scale)
{
	Vector3 origin = Transform.GetPosition();
	game->Gizmos.DrawRing(
		origin,
		right * scale * 0.8f,
		up * scale * 0.8f,
		Color(DirectX::Colors::White)
	);
}
