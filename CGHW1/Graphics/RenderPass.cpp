#include "RenderPass.h"
#include <algorithm>

void RenderPass::Execute(ID3D11DeviceContext* context)
{
	this->Bind(context);
	this->Draw();
}

bool RenderPass::AddBindable(Bindable* bind)
{
	auto it = std::find(bindables.begin(), bindables.end(), bind);
	if (it != bindables.end())
	{
		bindables.push_back(bind);
		return true;
	}
	return false;
}

bool RenderPass::RemoveBindable(Bindable* bind)
{
	auto it = std::find(bindables.begin(), bindables.end(), bind);
	if (it != bindables.end())
	{
		bindables.erase(it);
		return true;
	}
	return false;
}

bool RenderPass::AddComponent(GameComponent* comp)
{
	auto it = std::find(components.begin(), components.end(), comp);
	if (it == components.end())
	{
		components.push_back(comp);
		return true;
	}
	return false;
}

bool RenderPass::RemoveComponent(GameComponent* comp)
{
	auto it = std::find(components.begin(), components.end(), comp);
	if (it != components.end())
	{
		components.erase(it);
		return true;
	}
	return false;
}

void RenderPass::SetPassIndex(UINT index)
{
	this->passIndex = index;
}

UINT RenderPass::GetPassIndex() const
{
	return this->passIndex;
}

void RenderPass::Bind(ID3D11DeviceContext* context)
{
	for (auto b : bindables)
	{
		b->Bind(context);
	}
}

void RenderPass::Draw()
{
	for (auto c : components)
	{
		if (!c->IsEnabled())
			continue;

		c->Bind();
		c->Draw();
	}
}
