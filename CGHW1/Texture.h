#pragma once
#include "Bindable.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "StringHelper.h"
#include "Sampler.h"
#include <SimpleMath.h>
#include <assimp/material.h>


using namespace DirectX::SimpleMath;

enum class TextureStorageType
{
	Invalid,
	None,
	EmbeddedIndex,
	EmbeddedIndexCompressed,
	Embedded,
	EmbeddedCompressed,
	File
};

class Texture : public Bindable
{
public:
	Texture();
	Texture(Color color);
	Texture(std::string filePath);
	Texture(std::wstring filePath);
	HRESULT Initialize(ID3D11Device* device);
	ID3D11ShaderResourceView* GetTextureView() const;
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

protected:
	enum class TextureType
	{
		ColorArray,
		FilePath
	};

	struct TextureData
	{
		TextureType Type;
		std::wstring TexturePath;
		Color TextureColor;
	};

	TextureData textureData;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	Microsoft::WRL::ComPtr<ID3D11Resource> texture;
	Sampler sampler;

	HRESULT InitializeFromColor(ID3D11Device* device, const Color* colorData, UINT w, UINT h);
	HRESULT InitializeFromFile(ID3D11Device* device, std::wstring path);

};

