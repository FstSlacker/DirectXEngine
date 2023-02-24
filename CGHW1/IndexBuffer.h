#pragma once
#include <d3d11.h>
#include <wrl.h>

class IndexBuffer 
{
private:
	IndexBuffer(const IndexBuffer& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT bufferSize;

public:
	IndexBuffer() {}

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

	HRESULT Initialize(ID3D11Device* device, int* data, UINT indicesCount)
	{
		this->bufferSize = indicesCount;

		D3D11_BUFFER_DESC indexBufDesc = {};
		indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufDesc.CPUAccessFlags = 0;
		indexBufDesc.MiscFlags = 0;
		indexBufDesc.StructureByteStride = 0;
		indexBufDesc.ByteWidth = sizeof(int) * indicesCount;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = data;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		HRESULT res = device->CreateBuffer(&indexBufDesc, &indexData, this->buffer.GetAddressOf());

		return res;
	}

	void DestroyResources()
	{
		buffer->Release();
	}
};