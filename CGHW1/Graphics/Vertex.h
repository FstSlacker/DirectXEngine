#pragma once
#include <DirectXMath.h>
#include <SimpleMath.h>
#include <DirectXColors.h>

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

	Vertex(Vector3 position, Color color = DirectX::SimpleMath::Color(DirectX::Colors::White), Vector2 uv = Vector2::Zero, Vector3 normal = Vector3::Zero, Vector3 tangent = Vector3::Zero, Vector3 bitangent = Vector3::Zero)
	{
		Position = position;
		Color = color;
		UV = uv;
		Normal = normal;
		Tangent = tangent;
		Bitangent = bitangent;
	}

	Vector3 Position;
	Color Color;
	Vector2 UV;
	Vector3 Normal;
	Vector3 Tangent;
	Vector3 Bitangent;
};
