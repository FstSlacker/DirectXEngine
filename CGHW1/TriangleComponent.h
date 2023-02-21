#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>

class Game;
class GameComponent;

class TriangleComponent : public GameComponent
{
public:
	struct Vertex {
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT4 Color;
	};

protected:
	ID3D11InputLayout* layout;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode = nullptr;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode = nullptr;

	ID3D11Buffer* verticesBuffer;
	ID3D11Buffer* indicesBuffer;

	std::vector<Vertex> points;

	std::vector<int> indices;

public:

	TriangleComponent(Game* game, std::vector<Vertex> vertices, std::vector<int> indices);
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
};
