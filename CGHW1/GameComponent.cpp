#include "GameComponent.h"
#include <iostream>

GameComponent::GameComponent(Game* game, Transform3D transform) 
{
	this->game = game;
	this->Transform = transform;
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
