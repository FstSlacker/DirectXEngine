#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include "GameComponent.h"
#include "ConstantBuffer.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Game;
class GameComponent;

class MeshComponent : public GameComponent
{
protected:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

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
	virtual void SetShaders(VertexShader* vShader, PixelShader* pShader);
};
