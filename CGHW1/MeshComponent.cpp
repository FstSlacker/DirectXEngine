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

	transformMat.Data.WorldViewProjMatrix = DirectX::XMMatrixTranspose(
		Transform.GetTransformMatrix() * game->MainCamera->GetViewProjectionMatrix()
	);
	transformMat.Data.WorldMatrix = DirectX::XMMatrixTranspose(
		Transform.GetTransformMatrix()
	);

	transformMat.ApplyChanges(game->Gfx.GetContext());
	
	for (int i = 0; i < binds.size(); i++)
	{
		binds[i]->Bind(game->Gfx.GetContext());
	}

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

void MeshComponent::SetVertices(std::vector<VertexPositionTextureNormal> verts)
{
	points = verts;
}

void MeshComponent::SetIndices(std::vector<int> inds)
{
	indices = inds;
}

void MeshComponent::Initialize() {

	GameComponent::Initialize();

	HRESULT hr;

	hr = vertexBuffer.Initialize(game->Gfx.GetDevice(), points.data(), points.size());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize vertexBuffer");
	}

	hr = indexBuffer.Initialize(game->Gfx.GetDevice(), indices.data(), indices.size());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize indexBuffer");
	}

	hr = transformMat.Initialize(game->Gfx.GetDevice());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize transformMat");
	}

	binds.push_back(&vertexBuffer);
	binds.push_back(&indexBuffer);
	binds.push_back(&transformMat);
}