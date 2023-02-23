#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include "GameComponent.h"
#include "ConstantBuffer.h"

class Game;
class GameComponent;

class MeshComponent : public GameComponent
{
public:
	struct Vertex {
		Vector3 Position;
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

	ConstantBuffer<DirectX::XMMATRIX> transformMat;

	std::vector<Vertex> points;
	std::vector<int> indices;

public:

	MeshComponent(Game* game, Transform3D transform);
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;
};
