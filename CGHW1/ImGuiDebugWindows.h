#pragma once
#include "ImGuiWindow.h"
#include "GameComponent.h"
#include "Camera.h"

class Game;

class ImGuiGameInfoWindow : public ImGuiBaseWindow
{
protected:
	Game* game;

	void ShowGameComponent(GameComponent* comp);

public:
	ImGuiGameInfoWindow(Game* game);
	virtual void Bind() override;
};

class ImGuiGameCompWindow : public ImGuiBaseWindow
{
protected:
	GameComponent* gameComp;

	bool isWorldTransform;

	float position[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

	void GetTransform();
	void SetTransform();

public:
	ImGuiGameCompWindow(GameComponent* comp);
	virtual void Bind() override;
};

class ImGuiCameraWindow : public ImGuiGameCompWindow
{
protected:
	Camera* comp;

	float fov;
	float nearZ;
	float farZ;
	float aspectRatio;

	void GetCameraSettings();

public:
	ImGuiCameraWindow(Camera* comp);
	virtual void Bind() override;
};
