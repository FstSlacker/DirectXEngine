#include "PlaneComponent.h"
#include "Game.h"

PlaneComponent::PlaneComponent(Game* game, DirectX::XMFLOAT4 color) : MeshComponent(game)
{
	points = {
		Vertex{Vector3(0.5f, 0.5f, 0.5f), color, Vector2(1.0f, 1.0f)},
		Vertex{Vector3(-0.5f, -0.5f, 0.5f), color, Vector2(0.0f, 0.0f)},
		Vertex{Vector3(0.5f, -0.5f, 0.5f), color, Vector2(1.0f, 0.0f)},
		Vertex{Vector3(-0.5f, 0.5f, 0.5f), color, Vector2(0.0f, 1.0f)}
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
