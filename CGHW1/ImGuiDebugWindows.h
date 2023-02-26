#pragma once
#include "ImGuiWindow.h"
#include "GameComponent.h"

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
