#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <wrl.h>
#include "DepthStencil.h"
#include <SimpleMath.h>

class RenderTarget
{
public:
	bool Initialize(ID3D11Device* device, UINT width, UINT height);
	void BindAsTexture(ID3D11DeviceContext* context, UINT slot);
	void BindAsTarget(ID3D11DeviceContext* context);
	void BindAsTarget(ID3D11DeviceContext* context, DepthStencil& depthStencil);
	void Clear(ID3D11DeviceContext* context, DirectX::SimpleMath::Color c);
	ID3D11ShaderResourceView* GetTextureView() const;
	void DestroyResources();

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
};

