#include "MeshComponent.h"
#include "../Game.h"

MeshComponent::MeshComponent(Game* game) : GameComponent(game)
{
	this->Name = "Mesh_" + std::to_string(game->Components.size());
}

void MeshComponent::DestroyResources()
{
	GameComponent::DestroyResources();

	vertexBuffer->DestroyResources();
	indexBuffer->DestroyResources();
	transformMat.DestroyResources();
}

void MeshComponent::Bind()
{
	GameComponent::Bind();
	transformMat.Data.WorldViewProjMatrix = DirectX::XMMatrixTranspose(Transform.GetTransformMatrix() * game->MainCamera->GetViewProjectionMatrix());
	transformMat.Data.WorldMatrix = DirectX::XMMatrixTranspose(Transform.GetTransformMatrix());

	transformMat.ApplyChanges(game->Gfx.GetContext());

	transformMat.Bind(game->Gfx.GetContext());
	vertexBuffer->Bind(game->Gfx.GetContext());
	indexBuffer->Bind(game->Gfx.GetContext());
}

void MeshComponent::Draw()
{
	GameComponent::Draw();

	game->Gfx.GetContext()->DrawIndexed(indexBuffer->BufferSize(), 0, 0);
}

void MeshComponent::SetVertices(std::vector<Vertex> verts)
{
	vertices = verts;
}

void MeshComponent::SetIndices(std::vector<int> inds)
{
	indices = inds;
}

void MeshComponent::SetMaterial(Material* mat)
{
	if (this->material != nullptr)
	{
		this->material->RemoveFromComponent(*this);
	}

	mat->AttachToComponent(*this);
	this->material = mat;
}

Material* MeshComponent::GetMaterial() const
{
	return this->material;
}

void MeshComponent::Initialize() {

	GameComponent::Initialize();

	HRESULT hr;

	vertexBuffer = std::make_shared<VertexBuffer<Vertex>>();
	indexBuffer = std::make_shared<IndexBuffer>();

	hr = vertexBuffer->Initialize(game->Gfx.GetDevice(), vertices.data(), vertices.size());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize vertexBuffer");
	}

	hr = indexBuffer->Initialize(game->Gfx.GetDevice(), indices.data(), indices.size());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize indexBuffer");
	}

	hr = transformMat.Initialize(game->Gfx.GetDevice());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize transformMat");
	}

	transformMat.SetSlots(0U, 2U);
}