#pragma once
#include <vector>
#include "BindingPass.h"
#include "RenderJob.h"

class Graphics;

class RenderQueuePass : public BindingPass
{
public:
	void Execute(Graphics& gfx) override;
	void AddRenderJob(RenderJob* job);
	void Clear();

private:
	std::vector<RenderJob*> jobs;
};

