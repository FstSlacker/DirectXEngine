#include "MeshComponent.h"
#include "Game.h"

MeshComponent::MeshComponent(Game* game, Transform3D transform) : GameComponent(game, transform)
{

}

void MeshComponent::DestroyResources(){

	GameComponent::DestroyResources();

	layout->Release();
	vertexShader->Release();
	vertexShaderByteCode->Release();
	pixelShader->Release();
	pixelShaderByteCode->Release();
	verticesBuffer->Release();
	indicesBuffer->Release();
}

void MeshComponent::Draw(){

	GameComponent::Draw();

	UINT strides[1] = { sizeof(Vertex) };
	UINT offsets[1] = { 0 };

	game->Context->IASetInputLayout(layout);
	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->Context->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->Context->IASetVertexBuffers(0, 1, &verticesBuffer, strides, offsets);
	game->Context->VSSetShader(vertexShader, nullptr, 0);
	game->Context->PSSetShader(pixelShader, nullptr, 0);

	transformMat.Data = Transform.GetTransposedTransformMatrix();
	
	if (!transformMat.ApplyChanges())
	{
		std::cout << "Failed to apply transform mat!" << std::endl;
	}

	game->Context->VSSetConstantBuffers(0, 1, transformMat.GetAddressOf());



	game->Context->DrawIndexed(6, 0, 0);
}

void MeshComponent::Update() {
	GameComponent::Update();
}

void MeshComponent::FixedUpdate()
{
	GameComponent::FixedUpdate();
}

//void MeshComponent::SetPosition(Vector3 position)
//{
//	GameComponent::SetPosition(position);
//
//	transformMat.Data = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//	transformMat.Data = DirectX::XMMatrixTranspose(transformMat.Data);
//
//	if (!transformMat.ApplyChanges())
//	{
//		std::cout << "Failed to apply transform mat!" << std::endl;
//	}
//}

void MeshComponent::Initialize() {

	GameComponent::Initialize();

	//Set pipeline (compile and create shaders)
	ID3DBlob* errorVertexCode = nullptr;

	auto res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Shader file not found!" << std::endl;
		}
	}

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl", 
		nullptr /*macros*/,
		nullptr /*include*/, 
		"PSMain", 
		"ps_5_0", 
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
		0, 
		&pixelShaderByteCode, 
		&errorPixelCode);

	if (FAILED(res)) {
		std::cout << "Pixel shader not compile!" << std::endl;
	}

	game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	//Create vertex layout
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
	};

	game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);

	//Create vertex buffer
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &verticesBuffer);

	//Create index buffer
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * indices.size();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&indexBufDesc, &indexData, &indicesBuffer);

	//Init constant buffers
	transformMat.Initialize(game->Device.Get(), game->Context);
}