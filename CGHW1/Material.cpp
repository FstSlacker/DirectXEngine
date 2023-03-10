#include "Material.h"

HRESULT Material::Initialize(ID3D11Device* device)
{
	HRESULT hr = materialBuffer.Initialize(device);
	return hr;
}

void Material::Bind(ID3D11DeviceContext* context)
{
	materialBuffer.Data = Parameters;
	HRESULT hr = materialBuffer.ApplyChanges(context);
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to apply materialBuffer");
	}
	materialBuffer.Bind(context);
}

void Material::DestroyResources()
{
	//...
}
