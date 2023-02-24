#include "MeshComponent.h"
#include "Game.h"

MeshComponent::MeshComponent(Game* game, Transform3D transform) : GameComponent(game, transform)
{

}

void MeshComponent::DestroyResources(){

	GameComponent::DestroyResources();

	vertexBuffer.DestroyResources();
	indexBuffer.DestroyResources();
}

void MeshComponent::Draw(){

	GameComponent::Draw();

	UINT offsets[1] = { 0 };

	//game->Context->IASetInputLayout(vertexShader->GetInputLayout());
	//game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//game->Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	
	vertexBuffer.Bind(game->Context);//game->Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), offsets);
	indexBuffer.Bind(game->Context);
	vertexShader->Bind(game->Context);
	pixelShader->Bind(game->Context);

	sampler.Bind(game->Context);
	texture.Bind(game->Context);
	//game->Context->VSSetShader(vertexShader->GetShader(), nullptr, 0);
	//game->Context->PSSetShader(pixelShader->GetShader(), nullptr, 0);

	
	transformMat.Data = DirectX::XMMatrixTranspose(
		Transform.GetTransformMatrix() * (game->MainCamera->GetViewMatrix() * game->MainCamera->GetProjectionMatrix())
	);
	
	if (!transformMat.ApplyChanges(game->Context))
	{
		std::cout << "Failed to apply transform mat!" << std::endl;
	}

	game->Context->VSSetConstantBuffers(0, 1, transformMat.GetAddressOf());
	game->Context->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
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
	vertexBuffer.Initialize(game->Device.Get(), points.data(), points.size());

	//Create index buffer
	indexBuffer.Initialize(game->Device.Get(), indices.data(), indices.size());

	texture = Texture(L"./Textures/brich_wall.jpg");
	texture.Initialize(game->Device.Get());

	sampler.Initialize(game->Device.Get());

	//Init constant buffers
	transformMat.Initialize(game->Device.Get());
}