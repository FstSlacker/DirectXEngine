#include "Shaders.h"
#include "../Logs.h"

D3D11_INPUT_ELEMENT_DESC VertexShader::layoutDesc[6] = {
	D3D11_INPUT_ELEMENT_DESC {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	D3D11_INPUT_ELEMENT_DESC {"BITANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

VertexShader::VertexLayoutInfo VertexShader::layout = {

	layoutDesc,
	ARRAYSIZE(layoutDesc)
};

VertexShader::VertexShader(std::wstring shaderPath)
{
	this->shaderPath = shaderPath;
}

bool VertexShader::Initialize(ID3D11Device* device)
{
	ID3DBlob* errorCode = nullptr;
	HRESULT res;

	res = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE /*include*/,
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
			char* errors = (char*)(errorCode->GetBufferPointer());
			Logs::LogError(res, errors);
		}
		else
		{
			Logs::LogError(res, "Failed to compile VertexShader");
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
		Logs::LogError(res, "Failed to create VertexShader");
		return false;
	}
	
	res = device->CreateInputLayout(
		layout.Desc,
		layout.NumElements,
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		inputLayout.GetAddressOf()
	);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create InputLayout for VertexShader");
		return false;
	}

	return true;
}

void VertexShader::DestroyResources()
{
	shader.Reset();
	shaderByteCode.Reset();
	inputLayout.Reset();
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

void VertexShader::Bind(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(inputLayout.Get());
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(shader.Get(), nullptr, 0);
}

PixelShader::PixelShader(std::wstring shaderPath)
{
	this->shaderPath = shaderPath;
}

bool PixelShader::Initialize(ID3D11Device* device)
{
	ID3DBlob* errorCode;
	HRESULT res;

	res = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE /*include*/,
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
			char* errors = (char*)(errorCode->GetBufferPointer());
			Logs::LogError(res, errors);
		}
		else
		{
			Logs::LogError(res, "Failed to compile PixelShader");
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
		Logs::LogError(res, "Failed to create PixelShader");
		return false;
	}

	return true;
}

void PixelShader::Bind(ID3D11DeviceContext* context)
{
	context->PSSetShader(shader.Get(), nullptr, 0);
}

void PixelShader::DestroyResources()
{
	shader.Reset();
	shaderByteCode.Reset();
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* PixelShader::GetByteCode()
{
	return shaderByteCode.Get();
}

void NullPixelShader::Bind(ID3D11DeviceContext* context)
{
	context->PSSetShader(nullptr, nullptr, 0);
}

void NullPixelShader::DestroyResources()
{
	//...
}

GeometryShader::GeometryShader(std::wstring shaderPath)
{
	this->shaderPath = shaderPath;
}

ID3D11GeometryShader* GeometryShader::GetShader()
{
	return this->shader.Get();
}

ID3DBlob* GeometryShader::GetByteCode()
{
	return this->shaderByteCode.Get();
}

bool GeometryShader::Initialize(ID3D11Device* device)
{
	ID3DBlob* errorCode;
	HRESULT res;

	res = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE /*include*/,
		"GSMain",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderByteCode.GetAddressOf(),
		&errorCode);

	if (FAILED(res))
	{
		if (errorCode)
		{
			char* errors = (char*)(errorCode->GetBufferPointer());
			Logs::LogError(res, errors);
		}
		else
		{
			Logs::LogError(res, "Failed to compile GeometryShader");
		}
		return false;
	}

	res = device->CreateGeometryShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		nullptr,
		shader.GetAddressOf()
	);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create GeometryShader");
		return false;
	}

	return true;
}

void GeometryShader::Bind(ID3D11DeviceContext* context)
{
	context->GSSetShader(shader.Get(), nullptr, 0);
}

void GeometryShader::DestroyResources()
{
	this->shader.Reset();
	this->shaderByteCode.Reset();
}

ComputeShader::ComputeShader(std::wstring shaderPath)
{
	this->shaderPath = shaderPath;
}

ID3D11ComputeShader* ComputeShader::GetShader()
{
	return this->shader.Get();
}

ID3DBlob* ComputeShader::GetByteCode()
{
	return this->shaderByteCode.Get();
}

bool ComputeShader::Initialize(ID3D11Device* device)
{
	ID3DBlob* errorCode;
	HRESULT res;

	res = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE /*include*/,
		"CSMain",
		"cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderByteCode.GetAddressOf(),
		&errorCode);

	if (FAILED(res))
	{
		if (errorCode)
		{
			char* errors = (char*)(errorCode->GetBufferPointer());
			Logs::LogError(res, errors);
		}
		else
		{
			Logs::LogError(res, "Failed to compile ComputeShader");
		}
		return false;
	}

	res = device->CreateComputeShader(
		shaderByteCode->GetBufferPointer(),
		shaderByteCode->GetBufferSize(),
		nullptr,
		shader.GetAddressOf()
	);

	if (FAILED(res))
	{
		Logs::LogError(res, "Failed to create ComputeShader");
		return false;
	}

	return true;
}

void ComputeShader::Bind(ID3D11DeviceContext* context)
{
	context->CSSetShader(shader.Get(), nullptr, 0);
}

void ComputeShader::DestroyResources()
{
	this->shader.Reset();
	this->shaderByteCode.Reset();
}
