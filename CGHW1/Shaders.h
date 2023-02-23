#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <wrl.h>
#include <iostream>

class VertexShader
{
public:
	VertexShader(std::wstring shaderPath);
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device);
	void DestroyResources();

	ID3D11VertexShader* GetShader();
	ID3DBlob* GetByteCode();
	ID3D11InputLayout* GetInputLayout();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	Microsoft::WRL::ComPtr <ID3D11InputLayout> inputLayout;
	std::wstring shaderPath;
};


class PixelShader
{
public:
	PixelShader(std::wstring shaderPath);
	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device);
	void DestroyResources();

	ID3D11PixelShader* GetShader();
	ID3DBlob* GetByteCode();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	std::wstring shaderPath;
};