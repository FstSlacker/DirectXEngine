#pragma once
#include <d3d.h>
#include <d3d11.h>
#include "Bindable.h"

class DepthStencilBuffer : public Bindable
{
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

public:
	virtual bool Initialize(ID3D11Device* device, UINT width, UINT height);
	virtual void Bind(ID3D11DeviceContext* context) override;
	virtual void Clear(ID3D11DeviceContext* context);
	virtual void DestroyResources() override;

	ID3D11Texture2D* GetBuffer() const;
	ID3D11DepthStencilView* GetView() const;
};

