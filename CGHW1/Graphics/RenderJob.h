#pragma once
#include <vector>

#include "Bindable.h"
#include "../Components/GameComponent.h"

class RenderJob
{
public:
	void Execute(ID3D11DeviceContext* context);

	bool AddBindable(Bindable* bind);
	bool RemoveBindable(Bindable* bind);
	bool AddComponent(GameComponent* comp);
	bool RemoveComponent(GameComponent* comp);

	void SetQueueIndex(UINT index);
	UINT GetQueueIndex() const;

protected:
	virtual void Bind(ID3D11DeviceContext* context);
	virtual void Draw();

private:
	UINT queueIndex;
	std::vector<Bindable*> bindables;
	std::vector<GameComponent*> components;
};

