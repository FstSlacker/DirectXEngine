#pragma once
#include <d3d11.h>
#include <wrl/client.h>

class Bindable
{
public:
	virtual void Bind(ID3D11DeviceContext* context) = 0;
	virtual void DestroyResources() = 0;
};
