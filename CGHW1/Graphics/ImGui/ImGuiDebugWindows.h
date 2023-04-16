#pragma once
#include "ImGuiWindow.h"
#include "../../Components/GameComponent.h"
#include "../../Components/MeshComponent.h"
#include "../../Components/Camera.h"
#include "../../Components/Light.h"
#include "../../ResourceManager.h"

class Game;

class ImGuiGameInfoWindow : public ImGuiBaseWindow
{
protected:
	Game* game;

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

public:
	ImGuiGameCompWindow(GameComponent* comp);
	virtual void Bind() override;
};

class ImGuiResourcesWindow : public ImGuiBaseWindow
{
protected:
	ResourceManager* resourceManager;

public:
	ImGuiResourcesWindow(ResourceManager& resManager);
	virtual void Bind() override;
};
