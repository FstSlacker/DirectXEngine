#pragma once
#include "PlaneComponent.h"
#include "Game.h"

class RacketComponent : public PlaneComponent
{
public:
	float MoveSpeed = 1.0f;

	RacketComponent(Game* game, Transform3D transform, DirectX::XMFLOAT4 color);
	void MoveByX(float xValue);
	void Update() override;
};

