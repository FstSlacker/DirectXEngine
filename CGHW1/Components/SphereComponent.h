#pragma once
#include "MeshComponent.h"

class SphereComponent :
    public MeshComponent
{
public:
    SphereComponent(Game* game, float radius = 1.0f, size_t tesselation = 16U);
};

