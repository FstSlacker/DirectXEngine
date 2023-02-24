#pragma once
#include "GameComponent.h"
#include "Camera.h"

class CameraMoveComponent : public GameComponent
{
private:
	Vector2 prevMousePos;

public:
	CameraMoveComponent(Game* game, Transform3D transform);
	void Initialize() override;
	void Update() override;
};

