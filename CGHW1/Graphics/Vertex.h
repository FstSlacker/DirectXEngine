#pragma once
#include <DirectXMath.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct VertexPositionColor
{
	VertexPositionColor() {}

	VertexPositionColor(Vector3 position, Color color)
	{
		Position = position;
		Color = color;
	}

	Vector3 Position;
	Color Color;
};

struct VertexPositionColorTexture
{
	VertexPositionColorTexture() {}

	VertexPositionColorTexture(Vector3 position, Color color, Vector2 uv)
	{
		Position = position;
		Color = color;
		UV = uv;
	}

	Vector3 Position;
	Color Color;
	Vector2 UV;
};

struct VertexPositionTextureNormal
{
	VertexPositionTextureNormal() {}

	VertexPositionTextureNormal(Vector3 position, Vector2 uv, Vector3 normal)
	{
		Position = position;
		UV = uv;
		Normal = normal;
	}

	Vector3 Position;
	Vector2 UV;
	Vector3 Normal;
};

struct Vertex
{
	Vertex() {}

	Vertex(Vector3 position, Color color, Vector2 uv, Vector3 normal)
	{
		Position = position;
		Color = color;
		UV = uv;
		Normal = normal;
	}

	Vector3 Position;
	Color Color;
	Vector2 UV;
	Vector3 Normal;
};
