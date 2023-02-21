#include "GameComponent.h"
#include <iostream>

GameComponent::GameComponent(Game* game) {
	this->game = game;
}

void GameComponent::DestroyResources() {

}

void GameComponent::Draw() {
	std::cout << "Draw GC" << std::endl;
}

void GameComponent::Initialize() {

}

void GameComponent::Reaload() {

}

void GameComponent::Update() {

}