#pragma once
#include "../Graphics/Bindable.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Vertex.h"
#include <vector>

class Mesh
{
private:
	std::shared_ptr<VertexBuffer<Vertex>> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	std::vector<Vertex> vertices;
	std::vector<int> indices;

public:
	Mesh();

	void SetVertices(std::vector<Vertex> verts);
	void SetIndices(std::vector<int> inds);

	virtual bool Initialize(ID3D11Device* device);
	virtual void Draw(ID3D11DeviceContext* context);
	virtual void DestroyResources();
};

