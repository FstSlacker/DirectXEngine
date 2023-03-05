#include "ImGuiDebugWindows.h"
#include "Game.h"

void ImGuiGameInfoWindow::ShowGameComponent(GameComponent* comp)
{
	int childsCount = comp->Transform.GetChildsCount();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		| (childsCount == 0 ? ImGuiTreeNodeFlags_Leaf : 0);

	if (ImGui::TreeNodeEx(comp->Name.c_str(), flags))
	{
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
}

ImGuiGameInfoWindow::ImGuiGameInfoWindow(Game* game) : ImGuiBaseWindow("General Info")
{
	this->game = game;
}

void ImGuiGameInfoWindow::Bind()
{
	ImGui::Text("FPS: %.2f", 1.0f / game->DeltaTime);
	ImGui::Text("ImGui windows count: %d", game->ImGUI.GetWindowsCount());
	ImGui::Spacing();

	if (ImGui::TreeNode("Components"))
	{

		if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked())
			{
				game->ImGUI.AddWindow(new ImGuiCameraWindow(game->MainCamera));
			}
			
			ImGui::TreePop();
		}
		
		for (int i = 0; i < game->Components.size(); i++)
		{
			if (game->Components[i]->Transform.GetParent() == nullptr)
			{
				ShowGameComponent(game->Components[i]);
			}
		}

		/*if (ImGui::TreeNode("Game components"))
		{
			for (int i = 0; i < game->Components.size(); i++)
			{
				if (ImGui::Button(game->Components[i]->Name.c_str()))
				{
					game->ImGUI.AddWindow(new ImGuiGameCompWindow(game->Components[i]));
				}
			}
			ImGui::TreePop();
			ImGui::Spacing();
		}*/
		ImGui::TreePop();
	}
}


void ImGuiGameCompWindow::GetTransform()
{
	Vector3 p = gameComp->Transform.GetPosition();
	Vector3 r = gameComp->Transform.GetRotation();
	Vector3 s = gameComp->Transform.GetScale();

	position[0] = p.x;
	position[1] = p.y;
	position[2] = p.z;

	rotation[0] = r.x;
	rotation[1] = r.y;
	rotation[2] = r.z;

	scale[0] = s.x;
	scale[1] = s.y;
	scale[2] = s.z;
}

void ImGuiGameCompWindow::SetTransform()
{
	gameComp->Transform.SetPosition(Vector3(position[0], position[1], position[2]));
	gameComp->Transform.SetRotation(Vector3(rotation[0], rotation[1], rotation[2]));
	gameComp->Transform.SetScale(Vector3(scale[0], scale[1], scale[2]));
}

ImGuiGameCompWindow::ImGuiGameCompWindow(GameComponent* comp)
	: ImGuiBaseWindow(comp->Name)
{
	this->gameComp = comp;
}

void ImGuiGameCompWindow::Bind()
{
	GetTransform();

	ImGui::Text("Transform");
	if (ImGui::InputFloat3("Position", position)
		|| ImGui::InputFloat3("Rotation", rotation)
		|| ImGui::InputFloat3("Scale", scale))
	{
		SetTransform();
	}

	ImGui::Spacing();
	ImGui::Text("Collider");
	ImGui::Text(gameComp->Collider != nullptr ? "Yes" : "No");

	
}

void ImGuiCameraWindow::GetTransform()
{
	Vector3 p = comp->Transform.GetPosition();
	Vector3 r = comp->Transform.GetRotation();
	Vector3 s = comp->Transform.GetScale();

	position[0] = p.x;
	position[1] = p.y;
	position[2] = p.z;

	rotation[0] = r.x;
	rotation[1] = r.y;
	rotation[2] = r.z;

	scale[0] = s.x;
	scale[1] = s.y;
	scale[2] = s.z;
}

void ImGuiCameraWindow::SetTransform()
{
	comp->Transform.SetPosition(Vector3(position[0], position[1], position[2]));
	comp->Transform.SetRotation(Vector3(rotation[0], rotation[1], rotation[2]));
	comp->Transform.SetScale(Vector3(scale[0], scale[1], scale[2]));
}

void ImGuiCameraWindow::GetCameraSettings()
{
	fov = comp->GetFov();
	nearZ = comp->GetNear();
	farZ = comp->GetFar();
}

ImGuiCameraWindow::ImGuiCameraWindow(Camera* comp) : ImGuiBaseWindow("Camera")
{
	this->comp = comp;
}

void ImGuiCameraWindow::Bind()
{
	GetTransform();
	GetCameraSettings();

	ImGui::Text("Transform");
	if (ImGui::InputFloat3("Position", position)
		|| ImGui::InputFloat3("Rotation", rotation)
		|| ImGui::InputFloat3("Scale", scale))
	{
		SetTransform();
	}
	ImGui::Spacing();
	ImGui::Text("Camera settings");
	if (ImGui::SliderFloat("FOV", &fov, 0.01f, 360.0f))
	{
		comp->SetFov(fov);
	}
	if (ImGui::InputFloat("Near Z", &nearZ))
	{
		comp->SetNear(nearZ);
	}
	if (ImGui::InputFloat("Far Z", &farZ))
	{
		comp->SetFar(farZ);
	}
	
}
