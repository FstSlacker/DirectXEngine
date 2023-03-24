#pragma once
#include "RenderQueuePass.h"

class Graphics;
class LightComponent;

class ShadowMapPass : public RenderQueuePass
{

public:
	void BindLightSource(LightComponent& lightComponent);
	void Execute(Graphics& gfx) override;

private:
	LightComponent* lightComp;
};

