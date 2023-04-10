#include "MeshComponent.h"
#include "../Game.h"

MeshComponent::MeshComponent(Game* game) : GameComponent(game)
{
	this->Name = "Mesh_" + std::to_string(game->Components.size());
}

void MeshComponent::DestroyResources()
{
	GameComponent::DestroyResources();

	mesh->DestroyResources();
}

void MeshComponent::Draw()
{
	GameComponent::Draw();

	mesh->Draw(game->Gfx.GetContext());
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

bool MeshComponent::Initialize() {

	if (!GameComponent::Initialize())
		return false;

	if (!mesh->Initialize(game->Gfx.GetDevice()))
		return false;

	return true;
}