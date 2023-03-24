#pragma once
#include <vector>
#include "Bindable.h"
#include "Pass.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

class Graphics;

class BindingPass : public Pass
{
protected:
	void AddBind(Bindable* bind);
	void BindAll(Graphics& gfx);

	DepthStencil* depthStencil = nullptr;
	RenderTarget* renderTarget = nullptr;

private:
	void BindBuffers(Graphics& gfx);

	std::vector<Bindable*> binds;
};

