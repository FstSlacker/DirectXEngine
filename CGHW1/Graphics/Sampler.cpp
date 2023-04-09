#include "Sampler.h"
#include "../Logs.h"

bool Sampler::Initialize(ID3D11Device* device)
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
	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to init sampler");
		return false;
	}

	return true;
}

void Sampler::SetSlot(UINT slot)
{
	this->slotInd = slot;
}

void Sampler::Bind(ID3D11DeviceContext* context, UINT slot)
{
	context->PSSetSamplers(slot, 1, sampler.GetAddressOf());
}

void Sampler::Bind(ID3D11DeviceContext* context)
{
	context->PSSetSamplers(slotInd, 1, sampler.GetAddressOf());
}

void Sampler::DestroyResources()
{
	sampler.Reset();
}

bool ShadowSampler::Initialize(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC desc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	HRESULT res = device->CreateSamplerState(&desc, sampler.GetAddressOf());

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to init ShadowSampler");
		return false;
	}

	return true;
}
