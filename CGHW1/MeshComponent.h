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
#include "Material.h"

class Game;
class GameComponent;

class MeshComponent : public GameComponent
{
protected:
	struct TransformCbuf
	{
		DirectX::XMMATRIX WorldViewProjMatrix;
		DirectX::XMMATRIX WorldMatrix;
	};

	std::vector<Bindable*> binds;

	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;

	VSConstantBuffer<TransformCbuf> transformMat;

	std::vector<Vertex> vertices;
	std::vector<int> indices;

public:
	Material* Material;

	MeshComponent(Game* game);
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;

	void SetVertices(std::vector<Vertex> verts);
	void SetIndices(std::vector<int> inds);
};
