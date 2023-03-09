#include "PlaneComponent.h"
#include "Game.h"

PlaneComponent::PlaneComponent(Game* game) : MeshComponent(game)
{
	this->Name = "PlaneComponent_" + std::to_string(game->Components.size());

	Color color = Color(0.0f, 0.0f, 0.0f, 1.0f);

	points = {
		VertexPositionTextureNormal{Vector3(0.5f, 0.0f, 0.5f), Vector2(1.0f, 1.0f), Vector3::Up},
		VertexPositionTextureNormal{Vector3(-0.5f, 0.0f, -0.5f), Vector2(0.0f, 0.0f), Vector3::Up},
		VertexPositionTextureNormal{Vector3(0.5f, 0.0f, -0.5f), Vector2(1.0f, 0.0f), Vector3::Up},
		VertexPositionTextureNormal{Vector3(-0.5f, 0.0f, 0.5f), Vector2(0.0f, 1.0f), Vector3::Up}
	};

	indices = { 0,1,2, 1,0,3 };
}

void PlaneComponent::Update() 
{
	MeshComponent::Update();
}

void PlaneComponent::FixedUpdate()
{
	MeshComponent::FixedUpdate();
}
