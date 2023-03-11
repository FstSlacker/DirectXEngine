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
			MeshComponent* meshComp = dynamic_cast<MeshComponent*>(comp);
			if (meshComp != nullptr)
			{
				game->ImGUI.AddWindow(new ImGuiMeshCompWindow(meshComp));
			}
			else
			{
				game->ImGUI.AddWindow(new ImGuiGameCompWindow(comp));
			}
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

	if (ImGui::CollapsingHeader("Gizmos", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Show axis", &game->Gizmos.ShowAxis);
		ImGui::Checkbox("Show colliders", &game->Gizmos.ShowColliders);

		ImGui::Checkbox("Show grid XZ", &game->Gizmos.ShowGridXZ);
		ImGui::Checkbox("Show grid XY", &game->Gizmos.ShowGridXY);
		ImGui::Checkbox("Show grid YZ", &game->Gizmos.ShowGridYZ);
		ImGui::Spacing();
	}

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

		for (int i = 0; i < game->Light.GetLightSourcesCount(); i++)
		{
			if (ImGui::TreeNodeEx(game->Light.GetLightComponent(i)->Name.c_str(), ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked())
				{
					game->ImGUI.AddWindow(new ImGuiLightCompWindow(game->Light.GetLightComponent(i)));
				}

				ImGui::TreePop();
			}
		}

		ImGui::Separator();
		
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

bool ImGuiGameInfoWindow::IsOpened() const
{
	return true;
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
	ImGui::Checkbox("Is enabled", &gameComp->IsEnabled);
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
		if (ImGui::DragFloat3("Position", position, 0.25f)
			|| ImGui::DragFloat3("Rotation", rotation, 0.25f)
			|| ImGui::DragFloat3("Scale", scale, 0.25f))
		{
			SetTransform();
		}
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ColliderBase* colliderBase = gameComp->Collider;
		if (colliderBase != nullptr)
		{
			if (typeid(*colliderBase) == typeid(SphereCollider))
			{
				SphereCollider* collider = dynamic_cast<SphereCollider*>(colliderBase);
				ImGui::Text("Type: Sphere");
				ImGui::DragFloat("Radius", &collider->Radius);
				float offsets[3] = { collider->Offsets.x, collider->Offsets.y, collider->Offsets.z };
				if (ImGui::DragFloat3("Offsets", offsets))
				{
					collider->Offsets = Vector3(offsets[0], offsets[1], offsets[2]);
				}
			}
			else if (typeid(*colliderBase) == typeid(AABBCollider))
			{
				AABBCollider* collider = dynamic_cast<AABBCollider*>(colliderBase);
				ImGui::Text("Type: AABB");
				float size[3] = { collider->Size.x, collider->Size.y, collider->Size.z };
				if (ImGui::DragFloat3("Size", size))
				{
					collider->Size = Vector3(size[0], size[1], size[2]);
				}
				float offsets[3] = { collider->Offsets.x, collider->Offsets.y, collider->Offsets.z };
				if (ImGui::DragFloat3("Offsets", offsets))
				{
					collider->Offsets = Vector3(offsets[0], offsets[1], offsets[2]);
				}
			}
		}
		else
		{
			ImGui::Text("No");
		}
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

ImGuiLightCompWindow::ImGuiLightCompWindow(LightComponent* comp) : ImGuiGameCompWindow(comp)
{
	this->lightComp = comp;
}

void ImGuiLightCompWindow::Bind()
{
	ImGuiGameCompWindow::Bind();
	if (ImGui::CollapsingHeader("Global Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lightColor[3] = { Light::AmbientColor.x, Light::AmbientColor.y, Light::AmbientColor.z };
		if (ImGui::ColorEdit3("Ambient color", lightColor))
		{
			Light::AmbientColor = Color(lightColor[0], lightColor[1], lightColor[2]);
		}
		ImGui::DragFloat("Ambient intensity", &Light::AmbientIntensity, 0.01f, 0.0f, 10.0f);
		ImGui::Spacing();
	}

	std::string lightName = "<error name>";
	if (typeid(*lightComp) == typeid(PointLightComponent))
		lightName = "Point Light";
	else if (typeid(*lightComp) == typeid(DirectionalLightComponent))
		lightName = "Directional Light";
	else if (typeid(*lightComp) == typeid(SpotLightComponent))
		lightName = "Spot Light";

	if (ImGui::CollapsingHeader(lightName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		float lightColor[3] = { lightComp->LightColor.x, lightComp->LightColor.y, lightComp->LightColor.z };
		if (ImGui::ColorEdit3("Color", lightColor))
		{
			lightComp->LightColor = Color(lightColor[0], lightColor[1], lightColor[2]);
		}
		ImGui::DragFloat("Intensity", &lightComp->Intensity, 0.01f, 0.0f, 10.0f);

		if (typeid(*lightComp) == typeid(PointLightComponent))
		{
			ImGui::DragFloat("Range", &dynamic_cast<PointLightComponent*>(lightComp)->Range);
		}
		else if (typeid(*lightComp) == typeid(SpotLightComponent))
		{
			ImGui::DragFloat("ConeAngle", &dynamic_cast<SpotLightComponent*>(lightComp)->ConeAngle);
		}

		ImGui::Spacing();
	}
}


ImGuiMeshCompWindow::ImGuiMeshCompWindow(MeshComponent* comp) : ImGuiGameCompWindow(comp)
{
	this->meshComp = comp;
}

void ImGuiMeshCompWindow::Bind()
{
	ImGuiGameCompWindow::Bind();
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float color[3] = { meshComp->Material->AmbientColor.x, meshComp->Material->AmbientColor.y, meshComp->Material->AmbientColor.z };
		if (ImGui::ColorEdit3("AmbientColor", color))
		{
			meshComp->Material->AmbientColor = Color(color[0], color[1], color[2]);
		}

		color[0] = meshComp->Material->EmissiveColor.x; 
		color[1] = meshComp->Material->EmissiveColor.y;
		color[2] = meshComp->Material->EmissiveColor.z;

		if (ImGui::ColorEdit3("EmissiveColor", color))
		{
			meshComp->Material->EmissiveColor = Color(color[0], color[1], color[2]);
		}

		color[0] = meshComp->Material->DiffuseColor.x;
		color[1] = meshComp->Material->DiffuseColor.y;
		color[2] = meshComp->Material->DiffuseColor.z;

		if (ImGui::ColorEdit3("DiffuseColor", color))
		{
			meshComp->Material->DiffuseColor = Color(color[0], color[1], color[2]);
		}

		color[0] = meshComp->Material->SpecularColor.x;
		color[1] = meshComp->Material->SpecularColor.y;
		color[2] = meshComp->Material->SpecularColor.z;

		if (ImGui::ColorEdit3("SpecularColor", color))
		{
			meshComp->Material->SpecularColor = Color(color[0], color[1], color[2]);
		}

		ImGui::DragFloat("SpecularPower", &meshComp->Material->SpecularPower);

		if (meshComp->Material->DiffuseTexture != nullptr)
		{
			ImGui::Text("DiffuseTexture: YES");
		}
		else
		{
			ImGui::Text("DiffuseTexture: NO");
		}

		ImGui::Spacing();
	}
}