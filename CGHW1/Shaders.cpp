#include "Shaders.h"

VertexShader::VertexShader(std::wstring shaderPath)
{
	this->shaderPath = shaderPath;
}

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	ID3DBlob* errorCode = nullptr;
	HRESULT res;

	res = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderByteCode.GetAddressOf(),
		&errorCode);

	if (FAILED(res)) 
	{
		if (errorCode) 
		{
			char* compileErrors = (char*)(errorCode->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Vertex shader file not found:" << shaderPath.c_str() << std::endl;
		}
		return false;
	}

	res = device->CreateVertexShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		nullptr, 
		shader.GetAddressOf());

	if (FAILED(res))
	{
		std::cout << "Failed to create vertex shader (" << shaderPath.c_str() << ")" << std::endl;
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
		//D3D11_INPUT_ELEMENT_DESC {
		//	"TEXCOORD",
		//	0,
		//	DXGI_FORMAT_R32G32_FLOAT,
		//	0,
		//	D3D11_APPEND_ALIGNED_ELEMENT,
		//	D3D11_INPUT_PER_VERTEX_DATA,
		//	0}
	};

	res = device->CreateInputLayout(
		inputElements,
		2,
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		inputLayout.GetAddressOf()
	);

	if (FAILED(res))
	{
		std::cout << "Failed to create input layout for shader (" << shaderPath.c_str() << ")" << std::endl;
		return false;
	}

	return true;
}

void VertexShader::DestroyResources()
{
	shader->Release();
	shaderByteCode->Release();
	inputLayout->Release();
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* VertexShader::GetByteCode()
{
	return shaderByteCode.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return inputLayout.Get();
}




PixelShader::PixelShader(std::wstring shaderPath)
{
	this->shaderPath = shaderPath;
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	ID3DBlob* errorCode;
	HRESULT res;

	res = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderByteCode.GetAddressOf(),
		&errorCode);

	if (FAILED(res)) 
	{
		if (errorCode)
		{
			char* compileErrors = (char*)(errorCode->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Pixel shader file not found:" << shaderPath.c_str() << std::endl;
		}
		return false;
	}

	res = device->CreatePixelShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		nullptr, 
		shader.GetAddressOf()
	);

	if (FAILED(res))
	{
		std::cout << "Failed to create pixel shader (" << shaderPath.c_str() << ")" << std::endl;
	}

	return true;
}

void PixelShader::DestroyResources()
{
	shader->Release();
	shaderByteCode->Release();
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* PixelShader::GetByteCode()
{
	return shaderByteCode.Get();
}
