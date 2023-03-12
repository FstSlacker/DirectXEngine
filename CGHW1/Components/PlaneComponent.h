#pragma once
#include "MeshComponent.h"

class PlaneComponent : public MeshComponent
{
public:
	PlaneComponent(Game* game);
	void Update() override;
	void FixedUpdate() override;
};

