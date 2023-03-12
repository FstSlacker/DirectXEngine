#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler() {}

	HRESULT Initialize(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
};

