#pragma once
#include "MeshComponent.h"

class PlaneComponent : public MeshComponent
{
public:
	PlaneComponent(Game* game, Transform3D transform, DirectX::XMFLOAT4 color);
	void Update() override;
	void FixedUpdate() override;
};

