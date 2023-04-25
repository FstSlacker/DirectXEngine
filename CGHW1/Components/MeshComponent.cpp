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

void MeshComponent::DrawGui()
{
	GameComponent::DrawGui();

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Material* mat = this->GetMaterial();

		std::string renderQueueText = "Render queue: " + std::to_string(mat->GetQueueIndex());
		ImGui::Text(renderQueueText.c_str());

		float color[3] = { mat->AmbientColor.x, mat->AmbientColor.y, mat->AmbientColor.z };
		if (ImGui::ColorEdit3("AmbientColor", color))
		{
			mat->AmbientColor = Color(color[0], color[1], color[2]);
		}

		color[0] = mat->EmissiveColor.x;
		color[1] = mat->EmissiveColor.y;
		color[2] = mat->EmissiveColor.z;

		if (ImGui::ColorEdit3("EmissiveColor", color))
		{
			mat->EmissiveColor = Color(color[0], color[1], color[2]);
		}

		color[0] = mat->DiffuseColor.x;
		color[1] = mat->DiffuseColor.y;
		color[2] = mat->DiffuseColor.z;

		if (ImGui::ColorEdit3("DiffuseColor", color))
		{
			mat->DiffuseColor = Color(color[0], color[1], color[2]);
		}

		color[0] = mat->SpecularColor.x;
		color[1] = mat->SpecularColor.y;
		color[2] = mat->SpecularColor.z;

		if (ImGui::ColorEdit3("SpecularColor", color))
		{
			mat->SpecularColor = Color(color[0], color[1], color[2]);
		}

		ImGui::DragFloat("SpecularPower", &mat->SpecularPower);

		ImGui::Spacing();
		ImGui::Text("Textures:");

		if (ImGui::BeginTable("texturesTable", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Diffuse: ");

			ImGui::TableNextColumn();
			if (mat->DiffuseTexture != nullptr)
			{
				ImGui::Image(
					(void*)mat->DiffuseTexture->GetShaderResourceView(),
					ImVec2(30, 30)
				);
			}
			else
			{
				ImGui::Text("NONE");
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("NormalMap: ");

			ImGui::TableNextColumn();
			if (mat->NormalMapTexture != nullptr)
			{
				ImGui::Image(
					(void*)mat->NormalMapTexture->GetShaderResourceView(),
					ImVec2(30, 30)
				);
			}
			else
			{
				ImGui::Text("NONE");
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("SpecularMap: ");

			ImGui::TableNextColumn();
			if (mat->SpecularMapTexture != nullptr)
			{
				ImGui::Image(
					(void*)mat->SpecularMapTexture->GetShaderResourceView(),
					ImVec2(30, 30)
				);
			}
			else
			{
				ImGui::Text("NONE");
			}

			ImGui::EndTable();
		}

		ImGui::Spacing();
	}
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

Mesh* MeshComponent::GetMesh() const
{
	return this->mesh.get();
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