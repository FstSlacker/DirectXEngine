#pragma once
#include "GameComponent.h"
#include "Collision.h"

class MeshComponent;

class KatamariComponent : public GameComponent
{
private:
	GameComponent* rootComp;
	GameComponent* sphereComp;
	GameComponent* cameraRootComp;

	float currentSize;
	float targetSize;
	float growSpeed;

	Vector2 prevMousePos;

	void UpdateRotation();
	void OnCollisionEnter(const CollisionArgs& args);

public:
	float MoveSpeed;

	KatamariComponent(Game* game, float startSize = 1.0f);

	void Initialize() override;
	void Update() override;

};

