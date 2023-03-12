#include "Sampler.h"

HRESULT Sampler::Initialize(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT res = device->CreateSamplerState(&desc, sampler.GetAddressOf());
	return res;
}

void Sampler::Bind(ID3D11DeviceContext* context)
{
	context->PSSetSamplers(0, 1, sampler.GetAddressOf());
}

void Sampler::DestroyResources()
{
	//sampler->Release();
}
