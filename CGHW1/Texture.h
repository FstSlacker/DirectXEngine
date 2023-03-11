#pragma once
#include "Bindable.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "StringHelper.h"
#include "Sampler.h"
#include <SimpleMath.h>
#include <assimp/material.h>

using namespace DirectX::SimpleMath;

class Game;
class Graphics;

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
	friend class Graphics;

public:
	Texture(Game* game);
	Texture(Game* game, Color color);
	Texture(Game* game, std::string filePath);
	Texture(Game* game, std::wstring filePath);

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

	bool Initialize(ID3D11Device* device);
	HRESULT InitializeFromColor(ID3D11Device* device, const Color* colorData, UINT w, UINT h);
	HRESULT InitializeFromFile(ID3D11Device* device, std::wstring path);

};

