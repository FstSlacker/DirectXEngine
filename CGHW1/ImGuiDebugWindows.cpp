#include "ImGuiDebugWindows.h"
#include "Game.h"

ImGuiGameInfoWindow::ImGuiGameInfoWindow(Game* game) : ImGuiBaseWindow("General Info")
{
	this->game = game;
}

void ImGuiGameInfoWindow::Bind()
{
	ImGui::Text("FPS: %.2f", 1.0f / game->DeltaTime);
	ImGui::Text("ImGui windows count: %d", game->ImGUI.GetWindowsCount());
	ImGui::Spacing();
	if (ImGui::TreeNode("Game components"))
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
	ImGui::InputFloat3("Position", position);
	ImGui::InputFloat3("Rotation", rotation);
	ImGui::InputFloat3("Scale", scale);
	ImGui::Spacing();
	ImGui::Text("Collider");
	ImGui::Text(gameComp->Collider != nullptr ? "Yes" : "No");

	SetTransform();
}
