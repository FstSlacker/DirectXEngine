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

	//Graphics
	virtual void Draw();
	virtual void Bind();
	virtual bool Initialize();
	virtual void DestroyResources();

	virtual void DrawGizmos();
	virtual void DrawGizmosIcon(Vector3 forward, Vector3 up, float scale);

	//Game logic
	virtual void Reaload();
	virtual void Update();
	virtual void FixedUpdate();

	bool IsEnabled() const;
	void SetEnable(bool isEnable);

};

