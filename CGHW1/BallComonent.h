#pragma once
#include "PlaneComponent.h"
#include "Collision.h"

class BallComonent : public PlaneComponent
{
public:
	float MoveSpeed = 1.0f;
	Vector3 MoveDirection = Vector3::Up;

	BallComonent(Game* game, Vector3 position, float radius, DirectX::XMFLOAT4 color);
	void Update() override;
	void Move();

};

