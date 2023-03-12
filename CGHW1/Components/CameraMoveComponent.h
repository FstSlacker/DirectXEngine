#pragma once
#include "GameComponent.h"
#include "Camera.h"

class CameraMoveComponent : public GameComponent
{
private:
	Vector2 prevMousePos;

public:
	float MoveSpeed;

	CameraMoveComponent(Game* game);
	void Initialize() override;
	void Update() override;
};

