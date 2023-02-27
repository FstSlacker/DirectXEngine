#pragma once
#include "ImGuiWindow.h"
#include "GameComponent.h"
#include "Camera.h"

class Game;

class ImGuiGameInfoWindow : public ImGuiBaseWindow
{
protected:
	Game* game;

public:
	ImGuiGameInfoWindow(Game* game);
	virtual void Bind() override;
};

class ImGuiGameCompWindow : public ImGuiBaseWindow
{
protected:
	GameComponent* gameComp;
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

	void GetTransform();
	void SetTransform();

public:
	ImGuiGameCompWindow(GameComponent* comp);
	virtual void Bind() override;
};

class ImGuiCameraWindow : public ImGuiBaseWindow
{
protected:
	Camera* comp;
	float position[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

	float fov;
	float nearZ;
	float farZ;
	float aspectRatio;

	void GetTransform();
	void SetTransform();
	void GetCameraSettings();

public:
	ImGuiCameraWindow(Camera* comp);
	virtual void Bind() override;
};
