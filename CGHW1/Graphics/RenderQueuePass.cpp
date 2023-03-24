#include "RenderQueuePass.h"
#include "Graphics.h"

void RenderQueuePass::Execute(Graphics& gfx)
{
	BindAll(gfx);

	for (auto p : jobs)
	{
		p->Execute(gfx.GetContext());
	}
}

void RenderQueuePass::AddRenderJob(RenderJob* job)
{
	for (int i = 0; i < jobs.size(); i++)
	{
		if (jobs[i]->GetQueueIndex() > job->GetQueueIndex())
		{
			jobs.insert(jobs.begin() + i, job);
			return;
		}
	}
	jobs.insert(jobs.end(), job);
}

void RenderQueuePass::Clear()
{
	jobs.clear();
}
