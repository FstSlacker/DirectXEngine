#include "MeshComponent.h"
#include "Game.h"

MeshComponent::MeshComponent(Game* game, Transform3D transform) : GameComponent(game, transform)
{

}

void MeshComponent::DestroyResources(){

	GameComponent::DestroyResources();

	verticesBuffer->Release();
	indicesBuffer->Release();
}

void MeshComponent::Draw(){

	GameComponent::Draw();

	UINT strides[1] = { sizeof(Vertex) };
	UINT offsets[1] = { 0 };

	game->Context->IASetInputLayout(vertexShader->GetInputLayout());
	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->Context->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->Context->IASetVertexBuffers(0, 1, &verticesBuffer, strides, offsets);
	game->Context->VSSetShader(vertexShader->GetShader(), nullptr, 0);
	game->Context->PSSetShader(pixelShader->GetShader(), nullptr, 0);

	
	transformMat.Data = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixIdentity()
		* game->MainCamera->GetViewMatrix()
		* game->MainCamera->GetProjectionMatrix()
		* Transform.GetTransformMatrix()
	);
	
	if (!transformMat.ApplyChanges())
	{
		std::cout << "Failed to apply transform mat!" << std::endl;
	}

	game->Context->VSSetConstantBuffers(0, 1, transformMat.GetAddressOf());
	game->Context->DrawIndexed(indices.size(), 0, 0);
}

void MeshComponent::Update() {
	GameComponent::Update();
}

void MeshComponent::FixedUpdate()
{
	GameComponent::FixedUpdate();
}

void MeshComponent::SetShaders(VertexShader* vShader, PixelShader* pShader)
{
	vertexShader = vShader;
	pixelShader = pShader;
}

void MeshComponent::Initialize() {

	GameComponent::Initialize();

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