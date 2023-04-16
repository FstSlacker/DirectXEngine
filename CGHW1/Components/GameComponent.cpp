#include "GameComponent.h"
#include <iostream>
#include "../Game.h"

GameComponent::GameComponent(Game* game) : Transform(*this)
{
	this->game = game;
	this->isEnabled = true;
	this->Name = "GameComponent_" + std::to_string(game->Components.size());
	this->game->Components.push_back(this);
}

void GameComponent::DestroyResources() 
{
	//...
}

void GameComponent::Draw() 
{
	//...
}

void GameComponent::Bind()
{
	//...
}

bool GameComponent::Initialize() 
{
	//...
	return true;
}

void GameComponent::Reaload() 
{
	//...
}

void GameComponent::Update() 
{
	//...
}

void GameComponent::FixedUpdate()
{
	//...
}

bool GameComponent::IsEnabled() const
{
	return this->isEnabled;
}

void GameComponent::SetEnable(bool isEnable)
{
	if (isEnable != this->isEnabled)
	{
		this->isEnabled = isEnable;
		for (int i = 0; i < Transform.GetChildsCount(); i++)
		{
			Transform.GetChild(i)->GetGameComponent()->SetEnable(this->isEnabled);
		}
	}
}

void GameComponent::DrawGizmos()
{
	game->Gizmos.DrawAxis(this);
	game->Gizmos.DrawCollider(this);
}

void GameComponent::DrawGizmosIcon(Vector3 right, Vector3 up, float scale)
{
	Vector3 origin = Transform.GetPosition();
	game->Gizmos.DrawRing(
		origin,
		right * scale * 0.8f,
		up * scale * 0.8f,
		Color(DirectX::Colors::White)
	);
}

bool isWorldTransform = false;

void GameComponent::DrawGui()
{
	float position[3], rotation[3], scale[3];

	Vector3 p = isWorldTransform ? Transform.GetPosition() : Transform.GetLocalPosition();
	Vector3 r = isWorldTransform ? Transform.GetRotation() : Transform.GetLocalRotation();
	Vector3 s = isWorldTransform ? Transform.GetScale() : Transform.GetLocalScale();

	position[0] = p.x;
	position[1] = p.y;
	position[2] = p.z;

	rotation[0] = r.x;
	rotation[1] = r.y;
	rotation[2] = r.z;

	scale[0] = s.x;
	scale[1] = s.y;
	scale[2] = s.z;

	bool isCompEnabled = IsEnabled();
	if (ImGui::Checkbox("Is enabled", &isCompEnabled))
	{
		this->SetEnable(isCompEnabled);
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
		if (ImGui::DragFloat3("Position", position, 0.25f))
		{
			isWorldTransform ? 
				Transform.SetPosition(Vector3(position[0], position[1], position[2])) : 
				Transform.SetLocalPosition(Vector3(position[0], position[1], position[2]));
		}
		if (ImGui::DragFloat3("Rotation", rotation, 0.25f))
		{
			isWorldTransform ?
				Transform.SetRotation(Vector3(rotation[0], rotation[1], rotation[2])) :
				Transform.SetLocalRotation(Vector3(rotation[0], rotation[1], rotation[2]));
		}
		if(ImGui::DragFloat3("Scale", scale, 0.25f))
		{
			isWorldTransform ?
				Transform.SetScale(Vector3(scale[0], scale[1], scale[2])) :
				Transform.SetLocalScale(Vector3(scale[0], scale[1], scale[2]));
		}
		ImGui::Spacing();
	}
	if (this->Collider != nullptr)
	{
		if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ColliderBase* colliderBase = this->Collider;
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
