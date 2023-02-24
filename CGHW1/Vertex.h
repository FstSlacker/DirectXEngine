#pragma once
#include <DirectXMath.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct Vertex
{
public:
	Vertex()
	{
		Position = Vector3::Zero;
		Color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		UV = Vector2::Zero;
	}

	Vertex(Vector3 position, DirectX::XMFLOAT4 color, Vector2 uv)
	{
		Position = position;
		Color = color;
		UV = uv;
	}

	Vector3 Position;
	DirectX::XMFLOAT4 Color;
	Vector2 UV;
};
