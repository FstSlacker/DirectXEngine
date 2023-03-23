#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <wrl.h>

class RenderTarget
{
public:
	bool Initialize(ID3D11Device* device, UINT width, UINT height);
	void Bind(ID3D11DeviceContext* context);
	void DestroyResources();

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderView;
};

