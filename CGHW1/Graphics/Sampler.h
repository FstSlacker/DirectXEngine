#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	virtual HRESULT Initialize(ID3D11Device* device);
	void SetSlot(UINT slot);
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	UINT slotInd = 0;
};

class ShadowSampler : public Sampler
{
public:
	HRESULT Initialize(ID3D11Device* device) override;
};

