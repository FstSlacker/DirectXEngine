#pragma once
#include <d3d11.h>

class Graphics;

class Pass
{
public:
	virtual void Execute(Graphics& gfx) = 0;
};

