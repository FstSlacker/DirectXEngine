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
	enum class ProjectionMode
	{
		Perspective,
		Orthographic
	};

	Camera(Game* game);
	void SetFov(float angleDeg);
	void SetNear(float nearByZ);
	void SetFar(float farByZ);
	void SetAspectRatio(float aspectRatio);
	void SetProjectionMode(ProjectionMode mode);
	void SetOrthographicSize(float size);

	void DrawGizmos() override;

	float GetFov() const;
	float GetNear() const;
	float GetFar() const;
	float GetOrthographicSize() const;
	float GetAspectRatio() const;

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	ProjectionMode GetProjectionMode() const;

	const XMMATRIX& GetViewMatrix();
	const XMMATRIX& GetProjectionMatrix();
	const XMMATRIX GetViewProjectionMatrix();

	static Camera* Main;

protected:
	float fovDegrees;
	float nearZ;
	float farZ;
	float aspectRatio;
	float orthographicSize;

	ProjectionMode projMode;

	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;

	const XMVECTOR kDefaultForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR kDefaultUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

};

