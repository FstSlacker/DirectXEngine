#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "Bindable.h"
#include <d3dcompiler.h>
#include <string>
#include <wrl.h>
#include <iostream>

class VertexShader : public Bindable
{
public:
	VertexShader() {}
	VertexShader(std::wstring shaderPath);
	bool Initialize(ID3D11Device* device);

	ID3D11VertexShader* GetShader();
	ID3DBlob* GetByteCode();
	ID3D11InputLayout* GetInputLayout();

	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	Microsoft::WRL::ComPtr <ID3D11InputLayout> inputLayout;
	std::wstring shaderPath;
};


class PixelShader : public Bindable
{
public:
	PixelShader() {}
	PixelShader(std::wstring shaderPath);
	bool Initialize(ID3D11Device* device);

	ID3D11PixelShader* GetShader();
	ID3DBlob* GetByteCode();

	void Bind(ID3D11DeviceContext* context) override;
	void DestroyResources() override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	std::wstring shaderPath;
};