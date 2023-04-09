#pragma once
#include "Bindable.h"

class DepthStencilState : public Bindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> state;

public:
	bool Initialize(ID3D11Device* device, D3D11_DEPTH_STENCIL_DESC desc);
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;
};

