#pragma once
#include "Bindable.h"
#include "WICTextureLoader.h"
#include <string>
#include "Sampler.h"

class Texture : public Bindable
{
public:
	Texture() {}
	Texture(std::wstring imagePath);
	HRESULT Initialize(ID3D11Device* device);
	ID3D11ShaderResourceView* GetTextureView() const;
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	Sampler sampler;
	std::wstring texPath;
};

