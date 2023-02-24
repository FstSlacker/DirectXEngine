#include "Camera.h"

Camera::Camera(Transform3D transform, UINT width, UINT height)
{
    this->Transform = transform;
    this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
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

void Camera::UpdateViewMatrix()
{
    XMMATRIX rotMat = Transform.GetRotationMatrix();
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
