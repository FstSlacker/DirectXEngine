#pragma once
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Game;

class GameComponent
{
protected:
	Game* game;
	Vector3 position;

public:
	GameComponent(Game* game, Vector3 position);
	virtual void DestroyResources();
	virtual void Draw();
	virtual void Initialize();
	virtual void Reaload();
	virtual void Update();
};

