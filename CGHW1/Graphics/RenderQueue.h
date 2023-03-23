#pragma once
#include <vector>
#include "RenderPass.h"

class RenderQueue
{
public:
	void Execute(ID3D11DeviceContext* context);
	void AddPass(RenderPass* pass);
	void Clear();

private:
	std::vector<RenderPass*> passes;
};

