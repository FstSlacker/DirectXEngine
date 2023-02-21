#include "PlaneComponent.h"

PlaneComponent::PlaneComponent(Game* game, Vector3 position) : MeshComponent(game, position)
{
	
	/*points = {
		MeshComponent::Vertex{DirectX::XMFLOAT4(0.25f, 0.25f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		MeshComponent::Vertex{DirectX::XMFLOAT4(-0.25f, -0.25f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		MeshComponent::Vertex{DirectX::XMFLOAT4(0.25f, -0.25f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		MeshComponent::Vertex{DirectX::XMFLOAT4(-0.25f, 0.25f, 0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)}
	};*/
	points = {
		MeshComponent::Vertex{Vector3(0.25f, 0.25f, 0.5f) + position, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		MeshComponent::Vertex{Vector3(-0.25f, -0.25f, 0.5f) + position, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		MeshComponent::Vertex{Vector3(0.25f, -0.25f, 0.5f) + position, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
		MeshComponent::Vertex{Vector3(-0.25f, 0.25f, 0.5f) + position, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)}
	};

	indices = { 0,1,2, 1,0,3 };
}
