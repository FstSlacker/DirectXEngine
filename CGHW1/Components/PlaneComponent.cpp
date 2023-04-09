#include "PlaneComponent.h"
#include "../Game.h"

PlaneComponent::PlaneComponent(Game* game) : MeshComponent(game)
{
	this->Name = "Plane_" + std::to_string(game->Components.size());

	mesh = std::make_shared<Mesh>();

	Color color = Color(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<Vertex> vertices = {
		Vertex(Vector3(0.5f, 0.0f, 0.5f), color, Vector2(1.0f, 1.0f), Vector3::Up),
		Vertex(Vector3(-0.5f, 0.0f, -0.5f), color, Vector2(0.0f, 0.0f), Vector3::Up),
		Vertex(Vector3(0.5f, 0.0f, -0.5f), color, Vector2(1.0f, 0.0f), Vector3::Up),
		Vertex(Vector3(-0.5f, 0.0f, 0.5f), color, Vector2(0.0f, 1.0f), Vector3::Up)
	};

	std::vector<int> indices = { 2,1,0, 3,0,1 };

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);
}

void PlaneComponent::Update() 
{
	MeshComponent::Update();
}

void PlaneComponent::FixedUpdate()
{
	MeshComponent::FixedUpdate();
}
