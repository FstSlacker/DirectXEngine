#pragma once
#include "Bindable.h"
#include "../Logs.h"

template<typename T>
class StructuredBuffer
{

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	UINT numElements;

public:
	T* Data = nullptr;

	bool Initialize(ID3D11Device* device, UINT numElements)
	{
		this->numElements = numElements;

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufDesc.StructureByteStride = sizeof(T);
		bufDesc.ByteWidth = sizeof(T) * numElements;

		HRESULT res = device->CreateBuffer(&bufDesc, 0, buffer.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to initialize StructuredBuffer");
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = numElements;

		// Create the shader resource view.
		res = device->CreateShaderResourceView(buffer.Get(), &srvDesc, resourceView.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to create ShaderResourceView in StructuredBuffer");
			return false;
		}

		return true;
	}

	bool Apply(ID3D11DeviceContext* context)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT res = context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to apply StructuredBuffer", false);
			return false;
		}

		CopyMemory(mappedResource.pData, Data, sizeof(T) * numElements);
		context->Unmap(buffer.Get(), 0);

		return true;
	}

	virtual void DestroyResources()
	{
		this->buffer.Reset();
		this->resourceView.Reset();
	}

	ID3D11ShaderResourceView* GetShaderResourceView() const
	{
		return this->resourceView.Get();
	}

	ID3D11ShaderResourceView* const* GetShaderResourceViewAddressOf() const
	{
		return this->resourceView.GetAddressOf();
	}
};


template<typename T>
class VSStructuredBuffer : public StructuredBuffer<T>
{
public:
	void Bind(ID3D11DeviceContext* context, UINT slot) override
	{
		context->VSSetShaderResources(slot, 1, this->resourceView.GetAddressOf());
	}

	void Bind(ID3D11DeviceContext* context) override
	{
		throw std::exception("NotImplemented");
	}
};

template<typename T>
class UAVStructuredBuffer
{

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> unorderedAccessViewAppend;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> unorderedAccessViewCounter;

	UINT numElements;

public:
	T* Data = nullptr;

	bool Initialize(ID3D11Device* device, UINT numElements)
	{
		this->numElements = numElements;

		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bufDesc.CPUAccessFlags = 0;
		bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufDesc.StructureByteStride = sizeof(T);
		bufDesc.ByteWidth = sizeof(T) * numElements;

		HRESULT res = device->CreateBuffer(&bufDesc, 0, buffer.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to initialize UAVStructuredBuffer");
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = numElements;

		// Create the shader resource view.
		res = device->CreateShaderResourceView(buffer.Get(), &srvDesc, resourceView.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to create ShaderResourceView in UAVStructuredBuffer");
			return false;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = numElements;

		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;

		res = device->CreateUnorderedAccessView(buffer.Get(), &uavDesc, unorderedAccessViewAppend.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to create UnorderedAccessView in UAVStructuredBuffer");
			return false;
		}

		uavDesc.Buffer.Flags = 0;

		res = device->CreateUnorderedAccessView(buffer.Get(), &uavDesc, unorderedAccessViewCounter.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to create UnorderedAccessView in UAVStructuredBuffer");
			return false;
		}

		return true;
	}

	virtual void Apply(ID3D11DeviceContext* context)
	{
		context->UpdateSubresource(buffer.Get(), 0, nullptr, Data, 0, 0);
	}

	virtual void DestroyResources()
	{
		this->buffer.Reset();
		this->resourceView.Reset();

		this->unorderedAccessViewAppend.Reset();
		this->unorderedAccessViewCounter.Reset();
	}

	ID3D11ShaderResourceView* GetShaderResourceView() const
	{
		return this->resourceView.Get();
	}

	ID3D11UnorderedAccessView* GetUAVAppend() const
	{
		return this->unorderedAccessViewAppend.Get();
	}

	ID3D11UnorderedAccessView* GetUAVCounter() const
	{
		return this->unorderedAccessViewCounter.Get();
	}

	ID3D11ShaderResourceView* const* GetShaderResourceViewAddressOf() const
	{
		return this->resourceView.GetAddressOf();
	}

	ID3D11UnorderedAccessView** GetUAVAppendAddressOf()
	{
		return this->unorderedAccessViewAppend.GetAddressOf();
	}

	ID3D11UnorderedAccessView** GetUAVCounterAddressOf()
	{
		return this->unorderedAccessViewCounter.GetAddressOf();
	}
};
