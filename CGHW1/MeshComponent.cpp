#include "MeshComponent.h"
#include "Game.h"

MeshComponent::MeshComponent(Game* game) : GameComponent(game)
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
		binds[i]->Bind(game->Gfx.GetContext());
	}

	transformMat.Data = DirectX::XMMatrixTranspose(
		Transform.GetTransformMatrix() * (game->MainCamera->GetViewMatrix() * game->MainCamera->GetProjectionMatrix())
	);
	
	if (!transformMat.ApplyChanges(game->Gfx.GetContext()))
	{
		std::cout << "Failed to apply transform mat!" << std::endl;
	}

	game->Gfx.GetContext()->VSSetConstantBuffers(0, 1, transformMat.GetAddressOf());
	game->Gfx.GetContext()->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
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

void MeshComponent::SetVertices(std::vector<Vertex>& verts)
{
	points = verts;
}

void MeshComponent::SetIndices(std::vector<int>& inds)
{
	indices = inds;
}

void MeshComponent::Initialize() {

	GameComponent::Initialize();

	vertexBuffer.Initialize(game->Gfx.GetDevice(), points.data(), points.size());
	indexBuffer.Initialize(game->Gfx.GetDevice(), indices.data(), indices.size());

	binds.push_back(&vertexBuffer);
	binds.push_back(&indexBuffer);

	transformMat.Initialize(game->Gfx.GetDevice());
}