#pragma once
#include <SimpleMath.h>
#include <string>

#include "Transform.h"

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
	bool isEnabled;

public:
	GameComponent(Game* game);
	virtual void DestroyResources();
	virtual void Draw();
	virtual void Initialize();
	virtual void Reaload();
	virtual void Update();
	virtual void FixedUpdate();

	bool IsEnabled() const;
	void SetEnable(bool isEnable);

	virtual void DrawGizmos();
	virtual void DrawGizmosIcon(Vector3 forward, Vector3 up, float scale);
};

