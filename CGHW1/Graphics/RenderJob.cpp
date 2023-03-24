#include "RenderJob.h"
#include <algorithm>

void RenderJob::Execute(ID3D11DeviceContext* context)
{
	this->Bind(context);
	this->Draw();
}

bool RenderJob::AddBindable(Bindable* bind)
{
	auto it = std::find(bindables.begin(), bindables.end(), bind);
	if (it == bindables.end())
	{
		bindables.push_back(bind);
		return true;
	}
	return false;
}

bool RenderJob::RemoveBindable(Bindable* bind)
{
	auto it = std::find(bindables.begin(), bindables.end(), bind);
	if (it != bindables.end())
	{
		bindables.erase(it);
		return true;
	}
	return false;
}

bool RenderJob::AddComponent(GameComponent* comp)
{
	auto it = std::find(components.begin(), components.end(), comp);
	if (it == components.end())
	{
		components.push_back(comp);
		return true;
	}
	return false;
}

bool RenderJob::RemoveComponent(GameComponent* comp)
{
	auto it = std::find(components.begin(), components.end(), comp);
	if (it != components.end())
	{
		components.erase(it);
		return true;
	}
	return false;
}

void RenderJob::SetQueueIndex(UINT index)
{
	this->queueIndex = index;
}

UINT RenderJob::GetQueueIndex() const
{
	return this->queueIndex;
}

void RenderJob::Bind(ID3D11DeviceContext* context)
{
	for (auto b : bindables)
	{
		b->Bind(context);
	}
}

void RenderJob::Draw()
{
	for (auto c : components)
	{
		if (!c->IsEnabled())
			continue;

		c->Bind();
		c->Draw();
	}
}
