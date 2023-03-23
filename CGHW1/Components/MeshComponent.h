#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <directxmath.h>
#include <vector>
#include "GameComponent.h"

#include "../Graphics/Vertex.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/ConstantBuffer.h"
#include "../Graphics/Material.h"

class Game;
class GameComponent;

class MeshComponent : public GameComponent
{
protected:
	std::vector<Bindable*> binds;

	std::shared_ptr<VertexBuffer<Vertex>> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	TransformConstantBuffer transformMat;

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Material* material;

public:

	MeshComponent(Game* game);

	void Initialize() override;
	void Bind() override;
	void Draw() override;
	void DestroyResources() override;

	void SetVertices(std::vector<Vertex> verts);
	void SetIndices(std::vector<int> inds);

	void SetMaterial(Material* mat);
	Material* GetMaterial() const;
};
