#include "PlaneComponent.h"
#include "Game.h"

PlaneComponent::PlaneComponent(Game* game, Transform3D transform, DirectX::XMFLOAT4 color) : MeshComponent(game, transform)
{
	points = {
		MeshComponent::Vertex{Vector3(0.5f, 0.5f, 0.5f), color},
		MeshComponent::Vertex{Vector3(-0.5f, -0.5f, 0.5f), color},
		MeshComponent::Vertex{Vector3(0.5f, -0.5f, 0.5f), color},
		MeshComponent::Vertex{Vector3(-0.5f, 0.5f, 0.5f), color}
	};

	indices = { 0,1,2, 1,0,3 };
}

void PlaneComponent::Update() 
{
	MeshComponent::Update();
}
