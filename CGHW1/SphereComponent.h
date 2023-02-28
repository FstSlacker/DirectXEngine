#pragma once
#include "MeshComponent.h"

class SphereComponent :
    public MeshComponent
{
private:
    void CreateSphereMesh(size_t tslX, size_t tslY);

public:
    SphereComponent(Game* game, Transform3D transform, float radius = 1.0f, size_t tesselation = 16U);
};

