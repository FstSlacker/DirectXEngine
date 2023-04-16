#include "Camera.h"
#include "../Game.h"

Camera* Camera::Main = nullptr;

Camera::Camera(Game* game) : GameComponent(game)
{
    this->Name = "Camera";
    this->projMode = ProjectionMode::Perspective;
    this->fovDegrees = 90.0f;
    this->nearZ = 0.1f;
    this->farZ = 1000.0f;
    this->aspectRatio = 1.0f;
    this->orthographicSize = 10.0f;
}

void Camera::SetFov(float angleDeg)
{
    this->fovDegrees = angleDeg;
    UpdateProjectionMatrix();
}

void Camera::SetNear(float nearByZ)
{
    this->nearZ = nearByZ;
    UpdateProjectionMatrix();
}

void Camera::SetFar(float farByZ)
{
    this->farZ = farByZ;
    UpdateProjectionMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void Camera::SetProjectionMode(ProjectionMode mode)
{
    this->projMode = mode;
}

void Camera::SetOrthographicSize(float size)
{
    this->orthographicSize = size;
}

void Camera::DrawGizmos()
{
    if (Camera::Main != this) 
    {
        GameComponent::DrawGizmos();

        Quaternion orientation = Quaternion::CreateFromRotationMatrix(Transform.GetWorldRotationMatrix());
        Color color = Color(DirectX::Colors::Purple);

        if (this->projMode == ProjectionMode::Perspective)
        {
            game->Gizmos.DrawFrustrum(
                Transform.GetPosition(),
                orientation,
                GetProjectionMatrix(),
                color
            );
        }
        else
        {
            Vector3 center = Transform.GetForward() * ((farZ - nearZ) * 0.5f + nearZ) + Transform.GetPosition();
            float aspectWH = static_cast<float>(game->Display->ClientWidth) / static_cast<float>(game->Display->ClientHeight);
            Vector3 extends = Vector3(aspectWH * orthographicSize, orthographicSize, farZ - nearZ) * 0.5f;

            game->Gizmos.DrawOrientedBox(center, extends, orientation, color);
        }
    }
}

void Camera::DrawGui()
{
    GameComponent::DrawGui();

    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (Camera::Main != this)
        {
            if (ImGui::Button("SetAsMain"))
            {
                Camera::Main = this;
            }
        }

        int currentItem = static_cast<int>(this->GetProjectionMode());
        const char* items[] = { "Perspective", "Orthographic" };

        if (ImGui::Combo("Mode", &currentItem, items, IM_ARRAYSIZE(items)))
        {
            this->SetProjectionMode(static_cast<Camera::ProjectionMode>(currentItem));
        }
        if (static_cast<Camera::ProjectionMode>(currentItem) == Camera::ProjectionMode::Perspective)
        {
            float fov = this->GetFov();
            if (ImGui::SliderFloat("FOV", &fov, 0.01f, 360.0f))
            {
                this->SetFov(fov);
            }
        }
        else
        {
            float orthSize = this->GetOrthographicSize();
            if (ImGui::DragFloat("Size", &orthSize, 0.1f, 0.1f))
            {
                orthSize = orthSize < 0.1f ? 0.1f : orthSize;
                this->SetOrthographicSize(orthSize);
            }
        }

        if (ImGui::InputFloat("Near Z", &nearZ))
        {
            this->SetNear(nearZ);
        }
        if (ImGui::InputFloat("Far Z", &farZ))
        {
            this->SetFar(farZ);
        }

        ImGui::Spacing();
    }
}

float Camera::GetFov() const
{
    return this->fovDegrees;
}

float Camera::GetNear() const
{
    return this->nearZ;
}

float Camera::GetFar() const
{
    return this->farZ;
}

float Camera::GetOrthographicSize() const
{
    return this->orthographicSize;
}

float Camera::GetAspectRatio() const
{
    return this->aspectRatio;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
    return this->projMode;
}

const XMMATRIX& Camera::GetViewMatrix()
{
    UpdateViewMatrix();
    return this->viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix()
{
    UpdateProjectionMatrix();
    return this->projMatrix;
}

const XMMATRIX Camera::GetViewProjectionMatrix()
{
    return this->GetViewMatrix() * this->GetProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
    XMMATRIX rotMat = Transform.GetWorldRotationMatrix();
    XMVECTOR pos = Transform.GetPosition();

    XMVECTOR targetVector = XMVector3TransformCoord(this->kDefaultForwardVector, rotMat);
    targetVector += pos;
    XMVECTOR upDir = XMVector3TransformCoord(this->kDefaultUpVector, rotMat);

    this->viewMatrix = XMMatrixLookAtLH(pos, targetVector, upDir);
}

void Camera::UpdateProjectionMatrix()
{
    float aspectWH = aspectRatio;//static_cast<float>(game->Display->ClientWidth) / static_cast<float>(game->Display->ClientHeight);

    if (projMode == ProjectionMode::Perspective)
    {
        float fovRad = (fovDegrees / 360.0f) * XM_2PI;
        this->projMatrix = XMMatrixPerspectiveFovLH(fovRad, aspectWH, nearZ, farZ);
    }
    else if(projMode == ProjectionMode::Orthographic)
    {
        this->projMatrix = XMMatrixOrthographicLH(aspectWH * orthographicSize, 1.0f * orthographicSize, nearZ, farZ);
    }
    else
    {
        Logs::LogError("Camera projection mode not defined", false);
    }
}
