#pragma once
#include "RenderQueuePass.h"

class DirectionalLightComponent;
class Camera;
class DebugGizmos;
class Graphics;

class CascadeShadowMapPass : public RenderQueuePass
{
public:
	void BindDirectionalLight(DirectionalLightComponent& dirLight);
	void BindMainCamera(Camera& cam);
	void Execute(Graphics& gfx) override;

private:
	void CalculateOrthoProjections();
	DirectionalLightComponent* directionalLight;
	Camera* mainCamera;
	
};

