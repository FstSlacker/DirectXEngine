#include "SphereComponent.h"

#include <DirectXColors.h>
#include <math.h>

#include "../Game.h"

SphereComponent::SphereComponent(Game* game, float radius, size_t tesselation) 
	: MeshComponent(game)
{
	this->Name = "Sphere_" + std::to_string(game->Components.size());
	this->mesh = Mesh::CreateSphereMesh(tesselation, tesselation, radius);
}
