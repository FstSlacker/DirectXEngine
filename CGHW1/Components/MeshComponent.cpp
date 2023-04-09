#include "MeshComponent.h"
#include "../Game.h"

MeshComponent::MeshComponent(Game* game) : GameComponent(game)
{
	this->Name = "Mesh_" + std::to_string(game->Components.size());
}

void MeshComponent::DestroyResources()
{
	GameComponent::DestroyResources();

	//vertexBuffer->DestroyResources();
	//indexBuffer->DestroyResources();
	mesh->DestroyResources();
	transformMat.DestroyResources();
}

void MeshComponent::Bind()
{
	GameComponent::Bind();
	transformMat.Data.WorldViewProjMatrix = DirectX::XMMatrixTranspose(Transform.GetTransformMatrix() * Camera::Main->GetViewProjectionMatrix());
	transformMat.Data.WorldMatrix = DirectX::XMMatrixTranspose(Transform.GetTransformMatrix());

	transformMat.ApplyChanges(game->Gfx.GetContext());

	transformMat.Bind(game->Gfx.GetContext());
	//vertexBuffer->Bind(game->Gfx.GetContext());
	//indexBuffer->Bind(game->Gfx.GetContext());
}

void MeshComponent::Draw()
{
	GameComponent::Draw();

	mesh->Draw(game->Gfx.GetContext());
	//game->Gfx.GetContext()->DrawIndexed(indexBuffer->BufferSize(), 0, 0);
}

void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
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

	mesh->Initialize(game->Gfx.GetDevice());

	hr = transformMat.Initialize(game->Gfx.GetDevice());
	if (FAILED(hr))
	{
		Logs::LogError(hr, "Failed to initialize transformMat");
	}

	transformMat.SetSlots(0U, 2U);
}