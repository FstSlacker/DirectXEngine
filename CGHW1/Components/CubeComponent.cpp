#include "CubeComponent.h"
#include "../Game.h"

CubeComponent::CubeComponent(Game* game) : MeshComponent(game)
{
	this->Name = "Cube_" + std::to_string(game->Components.size());
	this->mesh = Mesh::CreateCubeMesh();
}
