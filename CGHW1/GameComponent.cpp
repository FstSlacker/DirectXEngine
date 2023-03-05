#include "GameComponent.h"
#include <iostream>
#include "Game.h"

GameComponent::GameComponent(Game* game) : Transform(*this)
{
	this->game = game;
	this->Name = "GameComponent_" + std::to_string(game->Components.size());
}

void GameComponent::DestroyResources() 
{

}

void GameComponent::Draw() 
{
	
}

void GameComponent::Initialize() 
{

}

void GameComponent::Reaload() 
{

}

void GameComponent::Update() 
{

}

void GameComponent::FixedUpdate()
{
	//...
}

//void GameComponent::SetPosition(Vector3 position)
//{
//	this->position = position;
//}
