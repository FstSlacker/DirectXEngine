#pragma once

#include <d3d11.h>
#include <wrl/client.h>

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* context;

public:

	T Data;

	ConstantBuffer() {}

	ID3D11Buffer* Get() const 
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const 
	{
		return buffer.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		this->context = context;

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = 0;
		bufDesc.StructureByteStride = 0;
		bufDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

		HRESULT res = device->CreateBuffer(&bufDesc, 0, buffer.GetAddressOf());

		return res;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT res = this->context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(res)) 
		{
			//...
			return false;
		}

		CopyMemory(mappedResource.pData, &Data, sizeof(T));
		this->context->Unmap(buffer.Get(), 0);
		return true;
	}
};