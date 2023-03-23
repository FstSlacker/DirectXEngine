#pragma once
#include <d3d11.h>

class GraphicResource
{
public:
	virtual bool Initialize(ID3D11Device* device) = 0;
};
