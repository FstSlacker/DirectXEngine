#include "DepthStencilState.h"
#include "../Logs.h"

bool DepthStencilState::Initialize(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC desc)
{
    HRESULT hr = device->CreateDepthStencilState(&desc, state.GetAddressOf());

    if (FAILED(hr))
    {
        Logs::LogError(hr, "Failed to create DepthStencilState");
        return false;
    }
    return true;
}

void DepthStencilState::Bind(ID3D11DeviceContext* context)
{
    context->OMSetDepthStencilState(state.Get(), 0);
}

void DepthStencilState::DestroyResources()
{
    state.Reset();
}
