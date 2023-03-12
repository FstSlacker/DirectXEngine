#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include "GameComponent.h"

using namespace DirectX;

class Game;

class Camera : public GameComponent
{

	friend class Game;

public:
	Camera(Game* game, UINT width, UINT height);
	void SetFov(float angleDeg);
	void SetNear(float nearByZ);
	void SetFar(float farByZ);
	void SetAspectRatio(float aspectRatio);

	float GetFov() const;
	float GetNear() const;
	float GetFar() const;

	const XMMATRIX& GetViewMatrix();
	const XMMATRIX& GetProjectionMatrix();
	const XMMATRIX GetViewProjectionMatrix();

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

