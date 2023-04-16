#include "ImGuiDebugWindows.h"
#include "../../Game.h"

void ImGuiGameInfoWindow::ShowGameComponent(GameComponent* comp)
{
	int childsCount = comp->Transform.GetChildsCount();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		| (childsCount == 0 ? ImGuiTreeNodeFlags_Leaf : 0);

	bool isEnabledColor = comp->IsEnabled();

	if (!isEnabledColor) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

	if (ImGui::TreeNodeEx(comp->Name.c_str(), flags))
	{
		if (!isEnabledColor) ImGui::PopStyleColor();

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			game->ImGUI.AddWindow(new ImGuiGameCompWindow(comp));
		}
		for (int i = 0; i < childsCount; i++)
		{
			ShowGameComponent(comp->Transform.GetChild(i)->GetGameComponent());
		}
		ImGui::TreePop();
	}
	else
	{
		if (!isEnabledColor) ImGui::PopStyleColor();
	}
}

ImGuiGameInfoWindow::ImGuiGameInfoWindow(Game* game) : ImGuiBaseWindow("General Info")
{
	this->game = game;
}

void ImGuiGameInfoWindow::Bind()
{

	ImGui::Text("FPS: %.2f", 1.0f / game->DeltaTime);
	ImGui::Text("ImGui windows count: %d", game->ImGUI.GetWindowsCount());

	float backColor[3];
	backColor[0] = game->Gfx.BackgroundColor.x;
	backColor[1] = game->Gfx.BackgroundColor.y;
	backColor[2] = game->Gfx.BackgroundColor.z;

	if (ImGui::ColorEdit3("Background color", backColor))
	{
		game->Gfx.BackgroundColor = DirectX::SimpleMath::Color(backColor[0], backColor[1], backColor[2]);
	}
	ImGui::Spacing();

	if (ImGui::Button("Resources"))
	{
		game->ImGUI.AddWindow(new ImGuiResourcesWindow(game->Resources));
	}

	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Gizmos", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Objects");
		ImGui::Checkbox("Icons", &game->Gizmos.ShowObjectsIcons);
		ImGui::SameLine();
		ImGui::Checkbox("Objects", &game->Gizmos.ShowObjectsGizmos);

		ImGui::Text("Grid:");
		ImGui::Checkbox("XZ", &game->Gizmos.ShowGridXZ);
		ImGui::SameLine();
		ImGui::Checkbox("XY", &game->Gizmos.ShowGridXY);
		ImGui::SameLine();
		ImGui::Checkbox("YZ", &game->Gizmos.ShowGridYZ);
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (int i = 0; i < game->Components.size(); i++)
		{
			if (game->Components[i]->Transform.GetParent() == nullptr)
			{
				ShowGameComponent(game->Components[i]);
			}
		}
		ImGui::Spacing();
	}


	/*float texSize = 100.0f;

	ImGui::Text("Diffuse");
	ImGui::Image(
		(void*)game->RenderSystem.GetGBuffer()->rtDiffuse.GetShaderResourceView(),
		ImVec2(texSize, texSize),
		ImVec2(0, 0),
		ImVec2(1, 1),
		ImVec4(1, 1, 1, 1),
		ImVec4(1, 0, 0, 1)
	);
	ImGui::Text("Emissive");
	ImGui::Image(
		(void*)game->RenderSystem.GetGBuffer()->rtEmissive.GetShaderResourceView(),
		ImVec2(texSize, texSize),
		ImVec2(0, 0),
		ImVec2(1, 1),
		ImVec4(1, 1, 1, 1),
		ImVec4(1, 0, 0, 1)
	);
	ImGui::Text("Normals");
	ImGui::Image(
		(void*)game->RenderSystem.GetGBuffer()->rtNormals.GetShaderResourceView(),
		ImVec2(texSize, texSize),
		ImVec2(0,0),
		ImVec2(1,1),
		ImVec4(1,1,1,1),
		ImVec4(1,0,0,1)

	);
	ImGui::Text("Specular");
	ImGui::Image(
		(void*)game->RenderSystem.GetGBuffer()->rtSpecular.GetShaderResourceView(),
		ImVec2(texSize, texSize),
		ImVec2(0, 0),
		ImVec2(1, 1),
		ImVec4(1, 1, 1, 1),
		ImVec4(1, 0, 0, 1)

	);
	ImGui::Text("WorldPos");
	ImGui::Image(
		(void*)game->RenderSystem.GetGBuffer()->rtWorldPos.GetShaderResourceView(),
		ImVec2(texSize, texSize),
		ImVec2(0, 0),
		ImVec2(1, 1),
		ImVec4(1, 1, 1, 1),
		ImVec4(1, 0, 0, 1)
	);*/

}

bool ImGuiGameInfoWindow::IsOpened() const
{
	return true;
}

void ImGuiGameInfoWindow::Begin()
{
	ImGui::Begin(name.c_str(), &isOpened);
}

ImGuiGameCompWindow::ImGuiGameCompWindow(GameComponent* comp)
	: ImGuiBaseWindow(comp->Name)
{
	this->gameComp = comp;
}

void ImGuiGameCompWindow::Bind()
{
	gameComp->DrawGui();
}

ImGuiResourcesWindow::ImGuiResourcesWindow(ResourceManager& resManager) : ImGuiBaseWindow("Resources")
{
	this->resourceManager = &resManager;
}

void ImGuiResourcesWindow::Bind()
{
	if (ImGui::TreeNode("Shaders"))
	{
		UINT vsCount = resourceManager->GetCount<VertexShader>();

		for (int i = 0; i < vsCount; i++)
		{
			const std::string& text = resourceManager->GetResourceName<VertexShader>(i);
			ImGui::Text(text.c_str());
		}

		UINT psCount = resourceManager->GetCount<PixelShader>();

		for (int i = 0; i < psCount; i++)
		{
			const std::string& text = resourceManager->GetResourceName<PixelShader>(i);
			ImGui::Text(text.c_str());
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Materials"))
	{
		UINT matCount = resourceManager->GetCount<Material>();

		for (int i = 0; i < matCount; i++)
		{
			const std::string& text = resourceManager->GetResourceName<Material>(i);
			ImGui::Text(text.c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Textures"))
	{
		UINT texCount = resourceManager->GetCount<Texture>();

		for (int i = 0; i < texCount; i++)
		{
			const std::string& text = resourceManager->GetResourceName<Texture>(i);
			ImGui::Text(text.c_str());
		}

		ImGui::TreePop();
	}
}
