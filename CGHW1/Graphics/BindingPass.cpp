#include "BindingPass.h"
#include "Graphics.h"


void BindingPass::AddBind(Bindable* bind)
{
	this->binds.push_back(bind);
}

void BindingPass::BindAll(Graphics& gfx)
{
	this->BindBuffers(gfx);

	for (auto& b : binds)
	{
		b->Bind(gfx.GetContext());
	}
}

void BindingPass::BindBuffers(Graphics& gfx)
{
	if (renderTarget)
	{
		renderTarget->Clear(gfx.GetContext(), DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f));
	}
	if (depthStencil)
	{
		depthStencil->Clear(gfx.GetContext());
	}

	if (!depthStencil && !renderTarget)
	{
		gfx.BindRenderTarget();
	}
	else if(renderTarget && !depthStencil)
	{
		renderTarget->BindAsTarget(gfx.GetContext());
	}
	else if(renderTarget && depthStencil)
	{
		renderTarget->BindAsTarget(gfx.GetContext(), *depthStencil);
	}
	else
	{
		depthStencil->BindAsDepthStencil(gfx.GetContext());
	}
}
