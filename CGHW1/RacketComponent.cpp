#include "RacketComponent.h"
#include "Collision.h"

RacketComponent::RacketComponent(Game* game, Transform3D transform, DirectX::XMFLOAT4 color) : PlaneComponent(game, transform, color)
{
	Collider = new AABBCollider(this, Vector3::One);
}

void RacketComponent::MoveByX(float dir)
{
	float halfSizeX = Transform.GetScale().x * 0.5f;
	Vector3 pos = Transform.GetPosition();

	pos.x += dir * MoveSpeed * game->DeltaTime;

	if (pos.x + halfSizeX > 1.0f)
	{
		pos.x = 1.0f - halfSizeX;
	}
	else if (pos.x - halfSizeX < -1.0f)
	{
		pos.x = -1.0f + halfSizeX;
	}

	Transform.SetPosition(pos);
}

void RacketComponent::Update()
{
	PlaneComponent::Update();
}
