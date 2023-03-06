#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include "GameComponent.h"

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ConstantBuffer.h"
#include "Shaders.h"
#include "Texture.h"
#include "Sampler.h"
#include "Bindable.h"

class Game;
class GameComponent;

class MeshComponent : public GameComponent
{
protected:
	std::vector<Bindable*> binds;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

	ConstantBuffer<DirectX::XMMATRIX> transformMat;

	std::vector<Vertex> points;
	std::vector<int> indices;

public:

	MeshComponent(Game* game);
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;
	void SetShaders(VertexShader* vShader, PixelShader* pShader);
	void SetTexture(Texture* tex);
	void SetVertices(std::vector<Vertex>& verts);
	void SetIndices(std::vector<int>& inds);
};
