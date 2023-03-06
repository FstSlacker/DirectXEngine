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

	backColor[0] = game->Gfx.BackgroundColor.x;
	backColor[1] = game->Gfx.BackgroundColor.y;
	backColor[2] = game->Gfx.BackgroundColor.z;

	if (ImGui::ColorEdit3("Background color", backColor))
	{
		game->Gfx.BackgroundColor = DirectX::SimpleMath::Color(backColor[0], backColor[1], backColor[2]);
	}
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNodeEx(game->MainCamera->Name.c_str(), ImGuiTreeNodeFlags_Leaf))
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
		ImGui::Spacing();
	}
}


void ImGuiGameCompWindow::GetTransform()
{
	Vector3 p = isWorldTransform ? gameComp->Transform.GetPosition() : gameComp->Transform.GetLocalPosition();
	Vector3 r = isWorldTransform ? gameComp->Transform.GetRotation() : gameComp->Transform.GetLocalRotation();
	Vector3 s = isWorldTransform ? gameComp->Transform.GetScale() : gameComp->Transform.GetLocalScale();

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
	if (isWorldTransform)
	{
		gameComp->Transform.SetPosition(Vector3(position[0], position[1], position[2]));
		gameComp->Transform.SetRotation(Vector3(rotation[0], rotation[1], rotation[2]));
		gameComp->Transform.SetScale(Vector3(scale[0], scale[1], scale[2]));
	}
	else
	{
		gameComp->Transform.SetLocalPosition(Vector3(position[0], position[1], position[2]));
		gameComp->Transform.SetLocalRotation(Vector3(rotation[0], rotation[1], rotation[2]));
		gameComp->Transform.SetLocalScale(Vector3(scale[0], scale[1], scale[2]));
	}
}

ImGuiGameCompWindow::ImGuiGameCompWindow(GameComponent* comp)
	: ImGuiBaseWindow(comp->Name)
{
	this->gameComp = comp;
	this->isWorldTransform = false;
}

void ImGuiGameCompWindow::Bind()
{
	GetTransform();

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::RadioButton("World", isWorldTransform))
		{
			isWorldTransform = true;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Local", !isWorldTransform))
		{
			isWorldTransform = false;
		}
		if (ImGui::DragFloat3("Position", position)
			|| ImGui::DragFloat3("Rotation", rotation)
			|| ImGui::DragFloat3("Scale", scale))
		{
			SetTransform();
		}
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text(gameComp->Collider != nullptr ? "Yes" : "No");
		ImGui::Spacing();
	}


	
}


void ImGuiCameraWindow::GetCameraSettings()
{
	fov = comp->GetFov();
	nearZ = comp->GetNear();
	farZ = comp->GetFar();
}

ImGuiCameraWindow::ImGuiCameraWindow(Camera* comp) : ImGuiGameCompWindow(comp)
{
	this->comp = comp;
}

void ImGuiCameraWindow::Bind()
{
	ImGuiGameCompWindow::Bind();

	GetCameraSettings();

	if (ImGui::CollapsingHeader("Camera settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
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
		ImGui::Spacing();
	}

	
	
}
