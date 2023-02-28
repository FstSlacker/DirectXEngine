#include "MeshComponent.h"
#include "Game.h"

MeshComponent::MeshComponent(Game* game, Transform3D transform) : GameComponent(game, transform)
{
	//...
}

void MeshComponent::DestroyResources(){

	GameComponent::DestroyResources();

	vertexBuffer.DestroyResources();
	indexBuffer.DestroyResources();
}

void MeshComponent::Draw(){

	GameComponent::Draw();

	for (int i = 0; i < binds.size(); i++)
	{
		binds[i]->Bind(game->Context);
	}

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

void MeshComponent::Update() 
{
	GameComponent::Update();
}

void MeshComponent::FixedUpdate()
{
	GameComponent::FixedUpdate();
}

void MeshComponent::SetShaders(VertexShader* vShader, PixelShader* pShader)
{
	binds.push_back(vShader);
	binds.push_back(pShader);
}

void MeshComponent::SetTexture(Texture* tex)
{
	binds.push_back(tex);
}

void MeshComponent::Initialize() {

	GameComponent::Initialize();

	vertexBuffer.Initialize(game->Device.Get(), points.data(), points.size());
	indexBuffer.Initialize(game->Device.Get(), indices.data(), indices.size());

	binds.push_back(&vertexBuffer);
	binds.push_back(&indexBuffer);

	transformMat.Initialize(game->Device.Get());
}