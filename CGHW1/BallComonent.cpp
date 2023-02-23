#include "BallComonent.h"
#include "Collision.h"
#include "Game.h"

BallComonent::BallComonent(Game* game, Vector3 position, float radius, DirectX::XMFLOAT4 color) : PlaneComponent(game, Transform3D(position, Vector3::Zero, Vector3::One * radius * 2.0f), color)
{
	Collider = new AABBCollider(this, Vector3::One);

	points = {
		MeshComponent::Vertex{Vector3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4{color.x, color.y, 1.0f, 1.0f}},
		MeshComponent::Vertex{Vector3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4{color.x, color.y, 0.0f, 0.0f}},
		MeshComponent::Vertex{Vector3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4{color.x, color.y, 1.0f, 0.0f}},
		MeshComponent::Vertex{Vector3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT4{color.x, color.y, 0.0f, 1.0f}}
	};
}

void BallComonent::Update()
{
	PlaneComponent::Update();
}

void BallComonent::Move()
{
	Vector3 pos = Transform.GetPosition();
	Vector3 scale = Transform.GetScale();

	float radius = scale.x * 0.5f;

	MoveDirection.Normalize();
	pos += MoveDirection * (game->DeltaTime * MoveSpeed);

	if (pos.x + radius > 1.0f)
	{
		pos.x = 1.0f - radius;
		MoveDirection.x *= -1.0f;

	}
	else if (pos.x - radius < -1.0f)
	{
		pos.x = -1.0f + radius;
		MoveDirection.x *= -1.0f;
	}

	Transform.SetPosition(pos);
}
