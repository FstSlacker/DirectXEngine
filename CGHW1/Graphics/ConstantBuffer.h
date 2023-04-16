#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "Bindable.h"
#include "../Logs.h"

template<typename T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

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

	bool Initialize(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = 0;
		bufDesc.StructureByteStride = 0;
		bufDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

		HRESULT res = device->CreateBuffer(&bufDesc, 0, buffer.GetAddressOf());
		if (FAILED(res))
		{
			Logs::LogError(res, "Failed to initialize ConstantBuffer");
			return false;
		}

		return true;
	}

	virtual void Bind(ID3D11DeviceContext* context, UINT slot) {}
	virtual void Bind(ID3D11DeviceContext* context) {}

	bool Apply(ID3D11DeviceContext* context)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT res = context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (FAILED(res)) 
		{
			Logs::LogError(res, "Failed to apply ConstantBuffer", false);
			return false;
		}

		CopyMemory(mappedResource.pData, &Data, sizeof(T));
		context->Unmap(buffer.Get(), 0);

		return true;
	}

	virtual void DestroyResources() 
	{
		this->buffer.Reset();
	}

};

template<typename T>
class VSConstantBuffer : public ConstantBuffer<T>
{
private:
	UINT slotInd;

public:
	VSConstantBuffer() { slotInd = 0; }

	void SetSlot(UINT slot)
	{
		this->slotInd = slot;
	}

	virtual void Bind(ID3D11DeviceContext* context, UINT slot) override
	{
		context->VSSetConstantBuffers(slot, 1, this->buffer.GetAddressOf());
	}

	virtual void Bind(ID3D11DeviceContext* context) override
	{
		context->VSSetConstantBuffers(slotInd, 1, this->buffer.GetAddressOf());
	}
};

template<typename T>
class PSConstantBuffer : public ConstantBuffer<T>
{
private:
	UINT slotInd;

public:
	PSConstantBuffer() { slotInd = 0; }

	void SetSlot(UINT slot)
	{
		this->slotInd = slot;
	}

	virtual void Bind(ID3D11DeviceContext* context, UINT slot) override
	{
		context->PSSetConstantBuffers(slot, 1, this->buffer.GetAddressOf());
	}

	virtual void Bind(ID3D11DeviceContext* context) override
	{
		context->PSSetConstantBuffers(slotInd, 1, this->buffer.GetAddressOf());
	}
};


struct TransformCbuf
{
	DirectX::XMMATRIX WorldViewProjMatrix;
	DirectX::XMMATRIX WorldMatrix;
	DirectX::XMFLOAT4 ViewPosition;
	DirectX::XMMATRIX ViewMatrix;
	DirectX::XMMATRIX ProjMatrix;
};

//class TransformConstantBuffer : public ConstantBuffer<TransformCbuf>
//{
//private:
//	UINT vsSlotInd;
//	UINT psSlotInd;
//
//public:
//	TransformConstantBuffer()
//	{
//		this->vsSlotInd = 0;
//		this->psSlotInd = 0;
//	}
//
//	void SetSlots(UINT vsSlot, UINT psSlot)
//	{
//		this->vsSlotInd = vsSlot;
//		this->psSlotInd = psSlot;
//	}
//
//	virtual void Bind(ID3D11DeviceContext* context) override
//	{
//		context->VSSetConstantBuffers(vsSlotInd, 1, this->buffer.GetAddressOf());
//		context->PSSetConstantBuffers(psSlotInd, 1, this->buffer.GetAddressOf());
//	}
//
//	virtual void DestroyResources() override {}
//};