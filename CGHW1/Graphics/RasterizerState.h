#pragma once
#include "Bindable.h"

class RasterizerState : public Bindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;

public:
	bool Initialize(ID3D11Device* device, CD3D11_RASTERIZER_DESC = {});
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;
};