#include "RasterizerState.h"
#include "../Logs.h"

bool RasterizerState::Initialize(ID3D11Device* device, CD3D11_RASTERIZER_DESC desc)
{
	HRESULT res = device->CreateRasterizerState(&desc, rastState.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create RasterizationState");
		return false;
	}

	return true;
}

void RasterizerState::Bind(ID3D11DeviceContext* context)
{
	context->RSSetState(rastState.Get());
}

void RasterizerState::DestroyResources()
{
	rastState.Reset();
}