#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "Bindable.h"
#include <d3dcompiler.h>
#include <string>
#include <wrl.h>


class VertexShader : public Bindable
{
public:
	enum class VertexLayoutType : UINT
	{
		VertexPositionColor,
		VertexPositionTexture,
		VertexPositionColorTexture,
		VertexPositionTextureNormal,
		VectrxPositionColorTextureNormal
	};

	VertexShader(std::wstring shaderPath, VertexLayoutType layoutType = VertexLayoutType::VertexPositionColorTexture);

	ID3D11VertexShader* GetShader();
	ID3DBlob* GetByteCode();
	ID3D11InputLayout* GetInputLayout();

	bool Initialize(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

private:
	struct VertexLayoutInfo
	{
		D3D11_INPUT_ELEMENT_DESC* Desc;
		UINT NumElements;
	};


	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::wstring shaderPath;
	VertexLayoutType layoutType;

	static D3D11_INPUT_ELEMENT_DESC vertexPositionColorLayout[2];
	static D3D11_INPUT_ELEMENT_DESC	vertexPositionTextureLayout[2];
	static D3D11_INPUT_ELEMENT_DESC	vertexPositionColorTextureLayout[3];
	static D3D11_INPUT_ELEMENT_DESC	vertexPositionTextureNormalLayout[3];
	static D3D11_INPUT_ELEMENT_DESC	vertexPositionColorTextureNormalLayout[4];

	static VertexLayoutInfo layouts[5];
};


class PixelShader : public Bindable
{
public:
	PixelShader() {}
	PixelShader(std::wstring shaderPath);

	ID3D11PixelShader* GetShader();
	ID3DBlob* GetByteCode();

	bool Initialize(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

private:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	std::wstring shaderPath;
};