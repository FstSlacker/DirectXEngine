#pragma once
#include "DebugDraw.h"
#include <d3d.h>
#include <d3d11.h>
#include <memory>
#include <Effects.h>
#include <wrl.h>
#include <DirectXHelpers.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class Game;
class GameComponent;
class LightComponent;

class DebugGizmos
{
private:
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::DX11::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Game* game;

	void DrawLight(LightComponent* light);
	void DrawCollider(GameComponent* comp);
	void DrawAxis(GameComponent* comp);
	void DrawGrid();

public:
	bool ShowAxis;
	bool ShowColliders;
	bool ShowLightSources;

	bool ShowGridXZ;
	bool ShowGridXY;
	bool ShowGridYZ;

	Color CollidersColor;
	Color GridColor;

	DebugGizmos(Game* game);
	void Initialize();
	void Draw();
};

