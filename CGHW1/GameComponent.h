#pragma once
#include <SimpleMath.h>
#include "Transform.h"
#include <string>

using namespace DirectX::SimpleMath;

class Game;
class ColliderBase;

class GameComponent
{
public:
	std::string Name;
	Transform3D Transform;
	ColliderBase* Collider = nullptr;

protected:
	Game* game;
	Vector3 position;

public:
	GameComponent(Game* game, Transform3D transform);
	virtual void DestroyResources();
	virtual void Draw();
	virtual void Initialize();
	virtual void Reaload();
	virtual void Update();
	virtual void FixedUpdate();
};

