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
			if (dynamic_cast<MeshComponent*>(comp) != nullptr)
			{
				game->ImGUI.AddWindow(new ImGuiMeshCompWindow(dynamic_cast<MeshComponent*>(comp)));
			}
			else if (dynamic_cast<Camera*>(comp) != nullptr)
			{
				game->ImGUI.AddWindow(new ImGuiCameraWindow(dynamic_cast<Camera*>(comp)));
			}
			else if (dynamic_cast<LightComponent*>(comp) != nullptr)
			{
				game->ImGUI.AddWindow(new ImGuiLightCompWindow(dynamic_cast<LightComponent*>(comp)));
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
}

bool ImGuiGameInfoWindow::IsOpened() const
{
	return true;
}

void ImGuiGameInfoWindow::Begin()
{
	ImGui::Begin(name.c_str(), &isOpened);
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
	bool isCompEnabled = gameComp->IsEnabled();
	if (ImGui::Checkbox("Is enabled", &isCompEnabled))
	{
		gameComp->SetEnable(isCompEnabled);
	}
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
	if (gameComp->Collider != nullptr)
	{
		if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ColliderBase* colliderBase = gameComp->Collider;
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
			ImGui::Spacing();
		}
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

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentItem = static_cast<int>(comp->GetProjectionMode());
		const char* items[] = { "Perspective", "Orthographic" };

		if (ImGui::Combo("Mode", &currentItem, items, IM_ARRAYSIZE(items)))
		{
			comp->SetProjectionMode(static_cast<Camera::ProjectionMode>(currentItem));
		}
		if (static_cast<Camera::ProjectionMode>(currentItem) == Camera::ProjectionMode::Perspective)
		{
			if (ImGui::SliderFloat("FOV", &fov, 0.01f, 360.0f))
			{
				comp->SetFov(fov);
			}
		}
		else
		{
			float orthSize = comp->GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthSize, 0.1f, 0.1f))
			{
				orthSize = orthSize < 0.1f ? 0.1f : orthSize;
				comp->SetOrthographicSize(orthSize);
			}
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
			PointLightComponent* pLight = dynamic_cast<PointLightComponent*>(lightComp);

			ImGui::DragFloat("Range", &pLight->Range);

			ImGui::Spacing();

			ImGui::Text("Attenuation:");
			ImGui::DragFloat("Constant", &pLight->ConstantAttenuation);
			ImGui::DragFloat("Linear", &pLight->LinearAttenuation);
			ImGui::DragFloat("Quadric", &pLight->QuadricAttenuation);
		}
		else if (typeid(*lightComp) == typeid(SpotLightComponent))
		{
			SpotLightComponent* pLight = dynamic_cast<SpotLightComponent*>(lightComp);

			ImGui::DragFloat("Range", &pLight->Range);
			ImGui::DragFloat("ConeAngle", &pLight->ConeAngle);

			ImGui::Spacing();

			ImGui::Text("Attenuation:");
			ImGui::DragFloat("Constant", &pLight->ConstantAttenuation);
			ImGui::DragFloat("Linear", &pLight->LinearAttenuation);
			ImGui::DragFloat("Quadric", &pLight->QuadricAttenuation);
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
		Material* mat = meshComp->GetMaterial();

		std::string renderQueueText = "Render queue: " + std::to_string(mat->GetPassIndex());
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
					(void*)mat->DiffuseTexture->GetTextureView(),
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
					(void*)mat->NormalMapTexture->GetTextureView(),
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
					(void*)mat->SpecularMapTexture->GetTextureView(),
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
