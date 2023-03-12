#include "Camera.h"

Camera::Camera(Game* game, UINT width, UINT height) : GameComponent(game)
{
    this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    this->Name = "Camera";
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
    float fovRad = (fovDegrees / 360.0f) * XM_2PI;
    this->projMatrix = XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);
}
