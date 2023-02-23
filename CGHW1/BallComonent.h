#pragma once
#include "PlaneComponent.h"
#include "Collision.h"

class BallComonent : public PlaneComponent
{
public:
	Vector3 Velocity = Vector3(0.0f, 1.0f, 0.0f);

	BallComonent(Game* game, Vector3 position, float radius, DirectX::XMFLOAT4 color);
	void Update() override;

	void OnCollisionEnter(const CollisionArgs& collisionArgs);

};

