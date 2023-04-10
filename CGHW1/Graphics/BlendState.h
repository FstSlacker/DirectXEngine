#pragma once
#include "Bindable.h"

class BlendState : public Bindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> state;

public:
	bool Initialize(ID3D11Device* device, D3D11_BLEND_DESC desc);
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;
};
