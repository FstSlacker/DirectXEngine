#pragma once
#include "ImGuiWindow.h"
#include "../../Components/GameComponent.h"
#include "../../Components/MeshComponent.h"
#include "../../Components/Camera.h"
#include "../../Components/Light.h"

class Game;

class ImGuiGameInfoWindow : public ImGuiBaseWindow
{
protected:
	Game* game;
	GameComponent* selectedComp;

	void ShowGameComponent(GameComponent* comp);

public:
	ImGuiGameInfoWindow(Game* game);
	virtual void Bind() override;
	virtual bool IsOpened() const override;
	virtual void Begin() override;
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

class ImGuiLightCompWindow : public ImGuiGameCompWindow
{
protected:
	LightComponent* lightComp;

public:
	ImGuiLightCompWindow(LightComponent* comp);
	virtual void Bind() override;
};

class ImGuiMeshCompWindow : public ImGuiGameCompWindow
{
protected:
	MeshComponent* meshComp;

public:
	ImGuiMeshCompWindow(MeshComponent* comp);
	virtual void Bind() override;
};
