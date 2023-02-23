#include "BallComonent.h"
#include "Collision.h"
#include "Game.h"

BallComonent::BallComonent(Game* game, Vector3 position, float radius, DirectX::XMFLOAT4 color) : PlaneComponent(game, Transform3D(position, Vector3::Zero, Vector3::One * radius * 2.0f), color)
{
	Collider = new AABBCollider(this, Vector3::One);
	Collider->OnCollisionEnter.AddRaw(this, &BallComonent::OnCollisionEnter);
}

void BallComonent::Update()
{
	PlaneComponent::Update();

	Vector3 pos = Transform.GetPosition();
	Vector3 scale = Transform.GetScale();

	float radius = scale.x * 0.5f;

	pos += Velocity * game->DeltaTime;

	if (pos.x + radius > 1.0f)
	{
		pos.x = 1.0f - radius;
		Velocity.x *= -1.0f;

	}
	else if (pos.x - radius < -1.0f)
	{
		pos.x = -1.0f + radius;
		Velocity.x *= -1.0f;
	}

	if (pos.y + radius > 1.0f)
	{
		pos.y = 1.0f - radius;
		Velocity.y *= -1.0f;

	}
	else if (pos.y - radius < -1.0f)
	{
		pos.y = -1.0f + radius;
		Velocity.y *= -1.0f;
	}

	Transform.SetPosition(pos);
}

void BallComonent::OnCollisionEnter(const CollisionArgs& collisionArgs)
{
	Vector3 racketPos = collisionArgs.CollidedComponent->Transform.GetPosition();
	Vector3 racketScale = collisionArgs.CollidedComponent->Transform.GetScale();
	Vector3 pos = Transform.GetPosition();

	float radius = Transform.GetScale().x * 0.5f;
	float ySign = racketPos.y >= 0.0f ? 1.0f : -1.0f;

	pos.y = (1.0f - racketScale.y - radius) * ySign;

	Velocity = (pos - racketPos);
	Velocity.Normalize();

}
