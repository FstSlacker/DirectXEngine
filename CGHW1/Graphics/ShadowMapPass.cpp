#include "ShadowMapPass.h"
#include "../Components/Light.h"


void ShadowMapPass::BindLightSource(LightComponent& lightComponent)
{
	this->lightComp = &lightComponent;

	this->renderTarget = lightComp->GetRenderTargets()[0].get();
	this->depthStencil = lightComp->GetDepthBuffer();
}

void ShadowMapPass::Execute(Graphics& gfx)
{
	Camera* mainCamera = Camera::Main;
	Camera::Main = lightComp->GetRenderCameras()[0];

	RenderQueuePass::Execute(gfx);

	Camera::Main = mainCamera;
}
