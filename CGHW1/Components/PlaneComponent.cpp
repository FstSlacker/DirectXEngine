#include "PlaneComponent.h"
#include "../Game.h"

PlaneComponent::PlaneComponent(Game* game) : MeshComponent(game)
{
	this->Name = "Plane_" + std::to_string(game->Components.size());
	this->mesh = Mesh::CreatePlaneMesh();
}
