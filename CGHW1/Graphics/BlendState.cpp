#include "BlendState.h"
#include "../Logs.h"

bool BlendState::Initialize(ID3D11Device* device, D3D11_BLEND_DESC desc)
{
	HRESULT hr = device->CreateBlendState(&desc, state.GetAddressOf());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to create BlendState");
		return false;
	}

	return true;
}

void BlendState::Bind(ID3D11DeviceContext* context)
{
	//float factors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(state.Get(), nullptr, 0xffffffff);
}

void BlendState::DestroyResources()
{
	state.Reset();
}