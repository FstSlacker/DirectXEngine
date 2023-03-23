#pragma once
#include <vector>

#include "Bindable.h"
#include "../Components/GameComponent.h"

class RenderPass
{
public:
	void Execute(ID3D11DeviceContext* context);

	bool AddBindable(Bindable* bind);
	bool RemoveBindable(Bindable* bind);
	bool AddComponent(GameComponent* comp);
	bool RemoveComponent(GameComponent* comp);

	void SetPassIndex(UINT index);
	UINT GetPassIndex() const;

protected:
	virtual void Bind(ID3D11DeviceContext* context);
	virtual void Draw();

private:
	UINT passIndex;
	std::vector<Bindable*> bindables;
	std::vector<GameComponent*> components;
};

