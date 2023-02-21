#pragma once

class Game;

class GameComponent
{
protected:
	Game* game;

public:
	GameComponent(Game* game);
	virtual void DestroyResources();
	virtual void Draw();
	virtual void Initialize();
	virtual void Reaload();
	virtual void Update();
};

