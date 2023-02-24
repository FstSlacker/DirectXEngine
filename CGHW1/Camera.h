#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include "Transform.h"

using namespace DirectX;

class Game;

class Camera
{

	friend class Game;

public:
	Transform3D Transform;

	Camera(Transform3D transform, UINT width, UINT height);
	void SetFov(float angleDeg);
	void SetNear(float nearByZ);
	void SetFar(float farByZ);
	void SetAspectRatio(float aspectRatio);

	const XMMATRIX& GetViewMatrix();
	const XMMATRIX& GetProjectionMatrix();

protected:
	float fovDegrees = 90.0f;
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	float aspectRatio;

	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;

	const XMVECTOR kDefaultForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR kDefaultUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
};

