#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "Bindable.h"

template<class T>
class VertexBuffer : public Bindable
{
private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize;

public:
	VertexBuffer() {}

	ID3D11Buffer* Get() const
	{
		return this->buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return this->buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return this->bufferSize;
	}

	const UINT Stride() const
	{
		return *this->stride.get();
	}

	const UINT* StridePtr() const
	{
		return this->stride.get();
	}

	HRESULT Initialize(ID3D11Device* device, T* data, UINT verticesCount)
	{
		this->bufferSize = verticesCount;
		this->stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC vertexBufDesc = {};
		vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufDesc.CPUAccessFlags = 0;
		vertexBufDesc.MiscFlags = 0;
		vertexBufDesc.StructureByteStride = 0;
		vertexBufDesc.ByteWidth = sizeof(T) * verticesCount;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = data;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT res = device->CreateBuffer(&vertexBufDesc, &vertexData, this->buffer.GetAddressOf());

		return res;
	}

	void Bind(ID3D11DeviceContext* context) override
	{
		UINT offsets[1] = { 0 };
		context->IASetVertexBuffers(0, 1, this->buffer.GetAddressOf(), this->stride.get(), offsets);
	}

	void DestroyResources() override
	{
		buffer.Reset();
		stride.release();
	}

};
