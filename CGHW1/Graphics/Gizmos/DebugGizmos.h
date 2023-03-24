#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <memory>
#include <Effects.h>
#include <wrl.h>
#include <DirectXHelpers.h>
#include <SimpleMath.h>

#include "DebugDraw.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

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

	void DrawGrid();

public:
	bool ShowObjectsGizmos;
	bool ShowObjectsIcons;
	bool ShowGridXZ;
	bool ShowGridXY;
	bool ShowGridYZ;

	Color CollidersColor;
	Color GridColor;

	DebugGizmos(Game* game);
	void DrawCollider(GameComponent* comp);
	void DrawAxis(GameComponent* comp);
	void DrawRing(Vector3 origin, Vector3 axis1, Vector3 axis2, Color color = Color(DirectX::Colors::White));
	void DrawQuad(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, Color color = Color(DirectX::Colors::White));
	void DrawRay(Vector3 origin, Vector3 direction, Color color = Color(DirectX::Colors::White));
	void DrawSphere(Vector3 origin, float radius, Color color = Color(DirectX::Colors::White));
	void DrawFrustrum(Vector3 origin, Quaternion orientation, XMMATRIX projection, Color color = Color(DirectX::Colors::White));
	void DrawOrientedBox(Vector3 center, Vector3 extents, Quaternion orientation, Color color = Color(DirectX::Colors::White));
	void Initialize();
	void Draw();
};

