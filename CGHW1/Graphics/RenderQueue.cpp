#include "RenderQueue.h"

void RenderQueue::Execute(ID3D11DeviceContext* context)
{
	for (auto p : passes)
	{
		p->Execute(context);
	}
}

void RenderQueue::AddPass(RenderPass* pass)
{
	for (int i = 0; i < passes.size(); i++)
	{
		if (passes[i]->GetPassIndex() > pass->GetPassIndex())
		{
			passes.insert(passes.begin() + i, pass);
			return;
		}
	}
	passes.insert(passes.end(), pass);
}

void RenderQueue::Clear()
{
	passes.clear();
}
