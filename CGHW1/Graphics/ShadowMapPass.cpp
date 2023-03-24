#include "ShadowMapPass.h"
#include "../Components/Light.h"


void ShadowMapPass::BindLightSource(LightComponent& lightComponent)
{
	this->lightComp = &lightComponent;

	this->renderTarget = lightComp->GetRenderTarget();
	this->depthStencil = lightComp->GetDepthStencil();
}

void ShadowMapPass::Execute(Graphics& gfx)
{
	Camera* mainCamera = Camera::Main;
	Camera::Main = &lightComp->GetLightCamera();

	RenderQueuePass::Execute(gfx);

	Camera::Main = mainCamera;

}
