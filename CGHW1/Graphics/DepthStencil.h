#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <wrl.h>

class DepthStencil
{
public:
	enum class UsageFormat
	{
		DepthStencil,
		ShadowDepth
	};

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	DXGI_FORMAT GetUsageTypelessFormat(UsageFormat format);
	DXGI_FORMAT GetUsageTypedFormat(UsageFormat format);
	DXGI_FORMAT GetUsageColoredFormat(UsageFormat format);

public:
	virtual bool Initialize(ID3D11Device* device, UINT width, UINT height, UsageFormat format = DepthStencil::UsageFormat::DepthStencil);
	virtual void BindAsDepthStencil(ID3D11DeviceContext* context);
	virtual void Clear(ID3D11DeviceContext* context);
	virtual void DestroyResources();

	ID3D11Texture2D* GetBuffer() const;
	ID3D11DepthStencilView* GetView() const;
};

